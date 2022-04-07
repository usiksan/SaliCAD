/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Dialog for edit and test sheet expressions

  From left to right: help widget, script edit widget, param edit table widget
*/
#include "SdDExpressionEdit.h"
#include "SdWScriptEditor.h"
#include "SdWScriptHighlighter.h"
#include "script/SdScriptParserCalculator.h"

#include <QLineEdit>
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

SdDExpressionEdit::SdDExpressionEdit(const QString &script, QWidget *parent) :
  QDialog(parent),
  mDirty(false),
  mActive(false)
  {
  //Setup editor size
  resize( qMin( QGuiApplication::primaryScreen()->size().width(), 1600), 700 );

  //Main layout of dialog is vertical
  QVBoxLayout *vlay = new QVBoxLayout();

   //Programm title with label
   QHBoxLayout *hlay; // = new QHBoxLayout();
//   hlay->addWidget( new QLabel(tr("Programm title:")) );
//   hlay->addWidget( mTitle = new QLineEdit() );

//  vlay->addLayout( hlay );

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
    connect( mTextEdit, &SdWScriptEditor::textChanged, this, &SdDExpressionEdit::parse );
    //connect( this, &SdD3dModelProgrammEditor::parseCompleted, mHighlighter, &SdW3dModelProgrammHighlighter::rehighlight );

    tlay->addWidget( mTextEdit );

     //Error line with label (result of programm compile)
     hlay = new QHBoxLayout();
     hlay->addWidget( new QLabel(tr("Error:")) );
     hlay->addWidget( mError = new QLineEdit() );

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
   connect( mParamWidget, &QTableWidget::cellChanged, this, &SdDExpressionEdit::recalculation );

   //Stretch factor
   splitter->setStretchFactor( 0, 1 );
   splitter->setStretchFactor( 1, 3 );
   splitter->setStretchFactor( 2, 1 );

  //Buttons at dialog bottom
  QDialogButtonBox *dialogButtonBox = new QDialogButtonBox( QDialogButtonBox::Ok | QDialogButtonBox::Cancel );
  vlay->addWidget( dialogButtonBox );
  connect( dialogButtonBox->button(QDialogButtonBox::Ok), &QPushButton::clicked, this, &SdDExpressionEdit::accept );
  connect( dialogButtonBox->button(QDialogButtonBox::Cancel), &QPushButton::clicked, this, &SdDExpressionEdit::reject );

  setLayout( vlay );

  mParamWidget->setColumnCount( 2 );
  mParamWidget->setHorizontalHeaderLabels( {tr("Name"), tr("Value") } );

  mRefMap = new SdExpressionRefMap( mParamWidget );

  //Extract programm source
  if( !script.isEmpty() ) {
    mTextEdit->setPlainText( script );
    parse();
    recalculation();
    }

  //Show beginner guide for scripting language
  mHelp->helpTopic( QStringLiteral("script.htm") );
  mDirty = false;
  }



SdDExpressionEdit::~SdDExpressionEdit()
  {
  delete mRefMap;
  }




QString SdDExpressionEdit::scriptGet() const
  {
  return mTextEdit->toPlainText();
  }




void SdDExpressionEdit::parse()
  {
  if( !mActive ) {
    mActive = true;
    //Set flag that programm text changed
    mDirty = true;
    SdScriptParserCalculator parser( mRefMap, mParamWidget );
    mRefMap->parseBegin();
    mProgramm = parser.parse( mTextEdit->toPlainText() );
    mError->setText( parser.error() );
    mRefMap->parseEnd();
    mHighlighter->setNameLists( parser.variableNameList(), parser.functionNameList() );
    //Rehighlight recall text changing with emit signal
    //This signal handles by this function parse and happens cycling
    //To eliminate this we introduce active flag
    mHighlighter->rehighlight();
    mActive = false;
    }
  }




void SdDExpressionEdit::recalculation()
  {
  if( mProgramm && !mActive ) {
    mActive = true;
    //Build new part
    mProgramm->execute();
    mActive = false;
    }
  }




