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
#include "master3d/SdM3dParser.h"
#include "master3d/SdM3dProgramm.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QSplitter>
#include <QDialogButtonBox>

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
  tlay->addWidget( mTextEdit = new QTextEdit() );

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
