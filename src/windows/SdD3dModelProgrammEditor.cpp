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

SdD3dModelProgrammEditor::SdD3dModelProgrammEditor(const QString id, QWidget *parent) :
  QDialog(parent),
  mProgramm(nullptr)
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
  splitter->addWidget( mPreview = new SdWView3d( &mPart, this ) );

  QDialogButtonBox *dialogButtonBox = new QDialogButtonBox( QDialogButtonBox::Save | QDialogButtonBox::Close );
  vlay->addWidget( dialogButtonBox );

  setLayout( vlay );

  //Extract programm source
  if( !id.isEmpty() )
    mRich = sdObjectOnly<SdPItemRich>( SdObjectFactory::extractObject( id, false, parent ) );
  if( mRich == nullptr ) {
    //Create new empty programm
    mRich = new SdPItemRich();
    }
  else {
    mTitle->setText( mRich->getTitle() );
    mDescription->setText( mRich->paramGet(stdParam3dModelProgramm) );
    mTextEdit->setPlainText( mRich->contents() );
    }
  }




void SdD3dModelProgrammEditor::compile()
  {
  if( mProgramm != nullptr ) {
    delete mProgramm;
    mProgramm = nullptr;
    }

  SdM3dParser parser;

  mProgramm = parser.parse( mTextEdit->toPlainText(), &mPart );

  mError->setText( parser.error() );

  rebuild();
  }




void SdD3dModelProgrammEditor::rebuild()
  {
  if( mProgramm != nullptr ) {
    //Clear previously builded part
    mPart.clear();

    //Build new part
    mProgramm->execute();
    }
  }




void SdD3dModelProgrammEditor::parse()
  {
  SdM3dParser parser;
  auto ptr = parser.parse( mTextEdit->toPlainText(), nullptr );
  delete ptr;
  mHighlighter->setNameLists( parser.variableNameList(), parser.functionNameList() );
  mTextEdit->update();
  //mHighlighter->rehighlight();
  }
