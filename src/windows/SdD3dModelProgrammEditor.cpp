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
#include "SdW3dModelProgrammEditor.h"
#include "SdW3dModelProgrammHighlighter.h"
#include "master3d/SdM3dParser.h"
#include "master3d/SdM3dProgramm.h"
#include "objects/SdObjectFactory.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QSplitter>
#include <QDialogButtonBox>
#include <QFont>
#include <QMessageBox>

SdD3dModelProgrammEditor::SdD3dModelProgrammEditor(const QString id, QWidget *parent) :
  QDialog(parent),
  mProgramm(nullptr),
  mDirty(false),
  mActive(false)
  {
  QVBoxLayout *vlay = new QVBoxLayout();

  QHBoxLayout *hlay = new QHBoxLayout();
  hlay->addWidget( new QLabel(tr("Programm title:")) );
  hlay->addWidget( mTitle = new QLineEdit() );

  vlay->addLayout( hlay );

  hlay = new QHBoxLayout();
  hlay->addWidget( new QLabel(tr("Programm description:")) );
  hlay->addWidget( mDescription = new QLineEdit() );

  vlay->addLayout( hlay );

  QSplitter *splitter = new QSplitter();
  vlay->addWidget( splitter );

  QVBoxLayout *tlay = new QVBoxLayout();
  QFont font;
  font.setFamily("Courier");
  font.setFixedPitch(true);
  font.setPointSize(10);

  mTextEdit = new SdW3dModelProgrammEditor();
  mTextEdit->setAutoCompleteParenthesis(true);
  mTextEdit->setAutoIndentSpaceCount(2);
  mTextEdit->setFont( font );

  mHighlighter = new SdW3dModelProgrammHighlighter( mTextEdit->document() );
  mTextEdit->setHighlighter( mHighlighter );
  connect( mTextEdit, SIGNAL(rehighlightBlock(QTextBlock)), mHighlighter, SLOT(rehighlightBlock(QTextBlock)) );
  connect( mTextEdit, &SdW3dModelProgrammEditor::textChanged, this, &SdD3dModelProgrammEditor::parse );

  tlay->addWidget( mTextEdit );

  hlay = new QHBoxLayout();
  hlay->addWidget( new QLabel(tr("Error:")) );
  hlay->addWidget( mError = new QLineEdit() );

  tlay->addLayout( hlay );

  hlay = new QHBoxLayout();
  QPushButton *build = new QPushButton(tr("Build"));
  build->setToolTip( tr("Compile source text 3d model programm") );
  connect( build, &QPushButton::clicked, this, &SdD3dModelProgrammEditor::compile );
  hlay->addWidget( build );
  hlay->addStretch(1);

  tlay->addLayout( hlay );

  QWidget *editor = new QWidget();
  editor->setLayout( tlay );

  splitter->addWidget( editor );
  splitter->addWidget( mParamWidget = new QTableWidget() );
  connect( mParamWidget, &QTableWidget::cellChanged, this, &SdD3dModelProgrammEditor::rebuild );

  splitter->addWidget( mPreview = new SdWView3d( &mPart, this ) );

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
    }
  }




SdD3dModelProgrammEditor::~SdD3dModelProgrammEditor()
  {
  if( mRich ) delete mRich;
  if( mProgramm ) delete mProgramm;
  }




//!
//! \brief compile Compile 3d model programm and receiv compilation errors
//!
void SdD3dModelProgrammEditor::compile()
  {
  if( mProgramm != nullptr ) {
    delete mProgramm;
    mProgramm = nullptr;
    }

  mActive = true;

  mParamWidget->clear();
  mParamWidget->setColumnCount(2);
  mParamWidget->setHorizontalHeaderLabels( {tr("Parametr name"), tr("Parametr value") } );
  SdM3dParser parser(mParamWidget);

  mProgramm = parser.parse( mTextEdit->toPlainText(), &mPart );

  mError->setText( parser.error() );

  mActive = false;

  rebuild();
  }




//!
//! \brief rebuild Rebuild resultat model on model param changed
//!
void SdD3dModelProgrammEditor::rebuild()
  {
  if( mProgramm != nullptr && !mActive ) {
    //Clear previously builded part
    mPart.clear();

    //Build new part
    mProgramm->execute();
    }
  }




//!
//! \brief parse Parse programm when programm text changed. It need for
//!              programm color highlighting and autocompletion function
//!
void SdD3dModelProgrammEditor::parse()
  {
  //Set flag that programm text changed
  mDirty = true;
  SdM3dParser parser(nullptr);
  auto ptr = parser.parse( mTextEdit->toPlainText(), nullptr );
  delete ptr;
  mHighlighter->setNameLists( parser.variableNameList(), parser.functionNameList() );
  mTextEdit->update();
  //mHighlighter->rehighlight();
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
  SdObjectFactory::insertItemObject( mRich, mRich->write() );
  }




//We test if programm not saved
void SdD3dModelProgrammEditor::closeEvent(QCloseEvent *event)
  {
  if( mDirty ) {
    //Programm text changed, query for close without saving
    if( QMessageBox::question( this, tr("Warning!"), tr("Programm text changed! Do You want close editor without saving it?") ) != QMessageBox::Yes ) {
      //User want not close without saving
      event->ignore();
      return;
      }
    }
  QDialog::closeEvent( event );
  }


