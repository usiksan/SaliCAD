/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  3d model programm editor dialog.

  It allow enter, edit and test 3d model programm
*/
#include "SdD3dModelProgrammEditor.h"
#include "SdWScriptEditor.h"
#include "SdWScriptHighlighter.h"
#include "SdDPadMaster.h"
#include "script/SdScriptParser3d.h"
#include "script/SdScriptProgramm.h"
#include "objects/SdObjectFactory.h"
#include "objects/Sd3dGraphModel.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QSplitter>
#include <QDialogButtonBox>
#include <QFont>
#include <QMessageBox>
#include <QColorDialog>
#include <QGuiApplication>
#include <QScreen>

SdD3dModelProgrammEditor::SdD3dModelProgrammEditor(const QString id, QWidget *parent) :
  QDialog(parent),
  mRich(nullptr),
  mDirty(false),
  mActive(false)
  {
  //Setup editor size
  resize( qMin( QGuiApplication::primaryScreen()->size().width(), 1600), 700 );

  //Main layout of dialog is vertical
  QVBoxLayout *vlay = new QVBoxLayout();

   //Programm title with label
   QHBoxLayout *hlay = new QHBoxLayout();
   hlay->addWidget( new QLabel(tr("Programm title:")) );
   hlay->addWidget( mTitle = new QLineEdit() );

  vlay->addLayout( hlay );

   //Programm description with label
   hlay = new QHBoxLayout();
   hlay->addWidget( new QLabel(tr("Programm description:")) );
   hlay->addWidget( mDescription = new QLineEdit() );

  vlay->addLayout( hlay );

  //Central part of dialog is splitter
  QSplitter *splitter = new QSplitter();
  vlay->addWidget( splitter );

    //Programm text editor
    QVBoxLayout *tlay = new QVBoxLayout();
    QFont font;
    font.setFamily("Courier");
    font.setFixedPitch(true);
    font.setPointSize(10);

    //Editor himself
    mTextEdit = new SdWScriptEditor();
    mTextEdit->setAutoCompleteParenthesis(true);
    mTextEdit->setAutoIndentSpaceCount(2);
    mTextEdit->setFont( font );

    //Highlighter for editor
    mHighlighter = new SdWScriptHighlighter( mTextEdit->document() );
    mTextEdit->setHighlighter( mHighlighter );
    connect( mTextEdit, SIGNAL(rehighlightBlock(QTextBlock)), mHighlighter, SLOT(rehighlightBlock(QTextBlock)) );
    connect( mTextEdit, &SdWScriptEditor::textChanged, this, &SdD3dModelProgrammEditor::parse );
    connect( mTextEdit, &SdWScriptEditor::setLink, mHighlighter, &SdWScriptHighlighter::setLink );
    //connect( this, &SdD3dModelProgrammEditor::parseCompleted, mHighlighter, &SdW3dModelProgrammHighlighter::rehighlight );

    tlay->addWidget( mTextEdit );

     //Error line with label (result of programm compile)
     hlay = new QHBoxLayout();
     hlay->addWidget( new QLabel(tr("Error:")) );
     hlay->addWidget( mError = new QLineEdit() );

    tlay->addLayout( hlay );

     //Buttons for compile text programm
     hlay = new QHBoxLayout();
     QPushButton *build = new QPushButton(tr("Build"));
     build->setToolTip( tr("Compile source text 3d model programm") );
     connect( build, &QPushButton::clicked, this, &SdD3dModelProgrammEditor::compile );
     hlay->addWidget( build );
     hlay->addStretch(1);

    tlay->addLayout( hlay );

   QWidget *editor = new QWidget();
   editor->setLayout( tlay );

   //Help widget
   splitter->addWidget( mHelp = new SdWHelp() );
   connect( mTextEdit, &SdWScriptEditor::help, mHelp, &SdWHelp::helpTopic );

   //Text editor widget with buttons
   splitter->addWidget( editor );

   //Param table widget
   splitter->addWidget( mParamWidget = new QTableWidget() );
   connect( mParamWidget, &QTableWidget::cellChanged, this, &SdD3dModelProgrammEditor::rebuild );
   connect( mParamWidget, &QTableWidget::cellClicked, this, &SdD3dModelProgrammEditor::onCellClicked );

   //Part preview widget
   splitter->addWidget( mPreview = new SdWView3d( &mPart, this ) );

   //Stretch factor
   splitter->setStretchFactor( 0, 1 );
   splitter->setStretchFactor( 1, 3 );
   splitter->setStretchFactor( 2, 1 );
   splitter->setStretchFactor( 3, 2 );

  //Buttons at dialog bottom
  QDialogButtonBox *dialogButtonBox = new QDialogButtonBox( QDialogButtonBox::Save | QDialogButtonBox::Close );
  vlay->addWidget( dialogButtonBox );
  connect( dialogButtonBox->button(QDialogButtonBox::Save), &QPushButton::clicked, this, &SdD3dModelProgrammEditor::save );
  connect( dialogButtonBox->button(QDialogButtonBox::Close), &QPushButton::clicked, this, &SdD3dModelProgrammEditor::close );

  setLayout( vlay );

  //Extract programm source
  if( !id.isEmpty() )
    //Retrive rich object with id from local libary
    mRich = sdObjectOnly<SdPItemRich>( SdObjectFactory::extractObject( id, false, parent ) );

  if( mRich == nullptr ) {
    //Create new empty programm
    mRich = new SdPItemRich();
    }
  else {
    mTitle->setText( mRich->getTitle() );
    mDescription->setText( mRich->paramGet(stdParam3dModelProgramm) );
    mTextEdit->setPlainText( mRich->contents() );
    compile();
    mPreview->fitItem();
    mPreview->update();
    }

  //Show beginner guide for scripting language
  mHelp->helpTopic( QStringLiteral("script3d.htm") );
  mDirty = false;
  }




SdD3dModelProgrammEditor::SdD3dModelProgrammEditor(const QString &title, const QString &script, QWidget *parent) :
  QDialog(parent),
  mRich(nullptr),
  mParamWidget(nullptr),
  mDirty(false),
  mActive(false)
  {
  //Setup editor size
  resize( qMin( QGuiApplication::primaryScreen()->size().width(), 1600), 700 );

  setWindowTitle( tr("3d model editor") );

  //Main layout of dialog is vertical
  QVBoxLayout *vlay = new QVBoxLayout();

  //Programm title with label
  QLabel *titleLabel = new QLabel(tr("Part title: ") + title );
  titleLabel->setMaximumHeight( 25 );
  vlay->addWidget( titleLabel );

  //Central part of dialog is splitter
  QSplitter *splitter = new QSplitter();
  vlay->addWidget( splitter );

    //Programm text editor
    QVBoxLayout *tlay = new QVBoxLayout();
    QFont font;
    font.setFamily("Courier");
    font.setFixedPitch(true);
    font.setPointSize(10);

    //Editor himself
    mTextEdit = new SdWScriptEditor();
    mTextEdit->setAutoCompleteParenthesis(true);
    mTextEdit->setAutoIndentSpaceCount(2);
    mTextEdit->setFont( font );

    //Highlighter for editor
    mHighlighter = new SdWScriptHighlighter( mTextEdit->document() );
    mTextEdit->setHighlighter( mHighlighter );
    connect( mTextEdit, SIGNAL(rehighlightBlock(QTextBlock)), mHighlighter, SLOT(rehighlightBlock(QTextBlock)) );
    connect( mTextEdit, &SdWScriptEditor::textChanged, this, &SdD3dModelProgrammEditor::parse );
    connect( mTextEdit, &SdWScriptEditor::setLink, mHighlighter, &SdWScriptHighlighter::setLink );
    //connect( this, &SdD3dModelProgrammEditor::parseCompleted, mHighlighter, &SdW3dModelProgrammHighlighter::rehighlight );

    tlay->addWidget( mTextEdit );

     //Error line with label (result of programm compile)
     QHBoxLayout *hlay = new QHBoxLayout();
     hlay->addWidget( new QLabel(tr("Error:")) );
     hlay->addWidget( mError = new QLineEdit() );

    tlay->addLayout( hlay );

     //Buttons for compile text programm
     hlay = new QHBoxLayout();
     QPushButton *build = new QPushButton(tr("Build"));
     build->setToolTip( tr("Compile source text 3d model programm") );
     connect( build, &QPushButton::clicked, this, &SdD3dModelProgrammEditor::compile );
     hlay->addWidget( build );
     hlay->addStretch(1);

    tlay->addLayout( hlay );

   QWidget *editor = new QWidget();
   editor->setLayout( tlay );

   //Help widget
   splitter->addWidget( mHelp = new SdWHelp() );
   connect( mTextEdit, &SdWScriptEditor::help, mHelp, &SdWHelp::helpTopic );

   //Text editor widget with buttons
   splitter->addWidget( editor );

   //Part preview widget
   splitter->addWidget( mPreview = new SdWView3d( &mPart, this ) );

   //Stretch factor
   splitter->setStretchFactor( 0, 1 );
   splitter->setStretchFactor( 1, 3 );
   splitter->setStretchFactor( 2, 2 );

  //Buttons at dialog bottom
  QDialogButtonBox *dialogButtonBox = new QDialogButtonBox( QDialogButtonBox::Ok | QDialogButtonBox::Cancel );
  vlay->addWidget( dialogButtonBox );
  connect( dialogButtonBox->button(QDialogButtonBox::Ok), &QPushButton::clicked, this, &SdD3dModelProgrammEditor::accept );
  connect( dialogButtonBox->button(QDialogButtonBox::Cancel), &QPushButton::clicked, this, &SdD3dModelProgrammEditor::close );

  setLayout( vlay );

  //Extract programm source
  mTextEdit->setPlainText( script );
  compile();
  mPreview->fitItem();
  mPreview->update();

  //Show beginner guide for scripting language
  mHelp->helpTopic( QStringLiteral("script3d.htm") );
  mDirty = false;
  }




SdD3dModelProgrammEditor::~SdD3dModelProgrammEditor()
  {
  if( mRich ) delete mRich;
  }




QString SdD3dModelProgrammEditor::script() const
  {
  return mTextEdit->toPlainText();
  }




//!
//! \brief compile Compile 3d model programm and receiv compilation errors
//!
void SdD3dModelProgrammEditor::compile()
  {
  mActive = true;

  if( mParamWidget != nullptr ) {
    mParamWidget->clear();
    mParamWidget->setColumnCount(2);
    mParamWidget->setRowCount(0);
    mParamWidget->setHorizontalHeaderLabels( {tr("Parametr name"), tr("Parametr value") } );
    }

  mModel.clear();
  SdScriptParser3d parser( mParamWidget, &mModel );

  mProgramm = parser.parse3d( mTextEdit->toPlainText(), &mPart, &mModel );

  mError->setText( parser.error() );

  mActive = false;

  rebuild();
  }




//!
//! \brief rebuild Rebuild resultat model on model param changed
//!
void SdD3dModelProgrammEditor::rebuild()
  {
  if( mProgramm && !mActive ) {
    //Clear previously builded part
    mActive = true;
    mPart.clear();
    mModel.clear();

    //Build new part
    mProgramm->execute();
    mPart.insertChild( new Sd3dGraphModel(mModel), nullptr );

    //Update preview
    mPreview->update();
    mActive = false;
    }
  }




//!
//! \brief parse Parse programm when programm text changed. It need for
//!              programm color highlighting and autocompletion function
//!
void SdD3dModelProgrammEditor::parse()
  {
  static bool active = false;
  if( !active ) {
    active = true;
    //Set flag that programm text changed
    mDirty = true;
    SdScriptParser3d parser( nullptr, &mModel );
    auto ptr = parser.parse3d( mTextEdit->toPlainText(), nullptr, &mModel );
    mHighlighter->setNameLists( parser.variableNameList(), parser.functionNameList() );
    mTextEdit->setHelpMap( parser.functionsHelp() );
    //Rehighlight recall text changing with emit signal
    //This signal handles by this function parse and happens cycling
    //To eliminate this we introduce active flag
    mHighlighter->rehighlight();
    active = false;
    }
  }




//!
//! \brief save Save editing programm to library
//!
void SdD3dModelProgrammEditor::save()
  {
  //Check if programm name and programm description filled
  if( mTitle->text().isEmpty() ) {
    QMessageBox::warning( this, tr("Warning!"), tr("Programm title not defined! Enter programm title and repeate saving") );
    return;
    }

  if( mDescription->text().isEmpty() ) {
    QMessageBox::warning( this, tr("Warning!"), tr("Programm description not defined! Enter programm description and repeate saving") );
    return;
    }

  //Build new rich if not builded yet
  if( mRich == nullptr )
    mRich = new SdPItemRich();

  //Update RichText
  mRich->setTitle( mTitle->text(), tr("Update 3d programm") );
  mRich->paramSet( stdParam3dModelProgramm, mDescription->text(), nullptr );
  mRich->setContents( mTextEdit->toPlainText() );
  //Push to library
  SdObjectFactory::insertItemObject( mRich, mRich->jsonObjectTo() );
  mDirty = false;
  }




//!
//! \brief onCellClicked Called when user clicked on mParamWidget cell
//! \param row           Row on which was clicked
//! \param column        Column on which was clicked
//!
void SdD3dModelProgrammEditor::onCellClicked(int row, int column)
  {
  if( column == 1 ) {
    QTableWidgetItem *item = mParamWidget->item( row, column );
    if( item != nullptr ) {
      if( item->type() == SDM3D_INPUT_COLOR ) {
        //Show color selection dialog
        QColor color = QColorDialog::getColor( QColor(item->text()), this, mParamWidget->item(row,0)->text() );
        if( color.isValid() )
          item->setText( color.name() );
        return;
        }
      else if( item->type() == SDM3D_INPUT_PAD ) {
        //Show pad master dialog
        item->setText( SdDPadMaster::build( item->text(), this ) );
        return;
        }
      }
    }
  }




//We test if programm not saved
void SdD3dModelProgrammEditor::closeEvent(QCloseEvent *event)
  {
  if( mDirty ) {
    //Programm text changed, query for close without saving
    if( QMessageBox::question( this, tr("Warning!"), tr("Programm text changed! Do You want save edited text before closing?") ) == QMessageBox::Yes ) {
      //User want not close without saving
      event->ignore();
      return;
      }
    }
  QDialog::closeEvent( event );
  }




void SdD3dModelProgrammEditor::reject()
  {
  if( mDirty ) {
    //Programm text changed, query for close without saving
    if( QMessageBox::question( this, tr("Warning!"), tr("Programm text changed! Do You want save edited text before closing?") ) == QMessageBox::Yes ) {
      //User want not close without saving
      return;
      }
    }
  QDialog::reject();
  }
