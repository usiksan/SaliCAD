/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Paths
*/
#include "SdDOptionsPagePath.h"
#include "objects/SdEnvir.h"
#include "library/SvDir.h"
#include <QSettings>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QFileDialog>

SdDOptionsPagePath::SdDOptionsPagePath(QWidget *parent) :
  QWidget(parent)
  {
  setWindowTitle( tr("Paths") );

  QGridLayout *grid = new QGridLayout();
  QPushButton *but;

  grid->addWidget( new QLabel(tr("Current user home path:")), 0, 0 );
  grid->addWidget( mHomePath = new QLineEdit(), 0, 1 );
  mHomePath->setText( sdEnvir->mHomePath );
  grid->addWidget( but = new QPushButton( tr("Select...") ), 0, 2 );
  connect( but, &QPushButton::clicked, this, [this] () {
    QString str = QFileDialog::getExistingDirectory( this, tr("Current user home path"), mHomePath->text() );
    if( !str.isEmpty() )
      mHomePath->setText( str );
    });


  grid->addWidget( new QLabel(tr("Library path:")), 1, 0 );
  grid->addWidget( mLibraryPath = new QLineEdit(), 1, 1 );
  mLibraryPath->setText( sdEnvir->mLibraryPath );
  grid->addWidget( but = new QPushButton( tr("Select...") ), 1, 2 );
  connect( but, &QPushButton::clicked, this, [this] () {
    QString str = QFileDialog::getExistingDirectory( this, tr("Library path"), mLibraryPath->text() );
    if( !str.isEmpty() )
      mLibraryPath->setText( str );
    });


  grid->addWidget( new QLabel(tr("Pattern path:")), 2, 0 );
  grid->addWidget( mPatternPath = new QLineEdit(), 2, 1 );
  mPatternPath->setText( sdEnvir->mPatternPath );
  grid->addWidget( but = new QPushButton( tr("Select...") ), 2, 2 );
  connect( but, &QPushButton::clicked, this, [this] () {
    QString str = QFileDialog::getExistingDirectory( this, tr("Pattern path"), mPatternPath->text() );
    if( !str.isEmpty() )
      mPatternPath->setText( str );
    });


  grid->addWidget( new QLabel(tr("Category hierarchy file:")), 3, 0 );
  grid->addWidget( mCategoryPath = new QLineEdit(), 3, 1 );
  mCategoryPath->setText( sdEnvir->mCategoryPath );
  grid->addWidget( but = new QPushButton( tr("Select...") ), 3, 2 );
  connect( but, &QPushButton::clicked, this, [this] () {
    QString str = QFileDialog::getOpenFileName( this, tr("Category hierarchy file"), mCategoryPath->text(), QString("*" SD_CATEGORY_EXTENSION) );
    if( !str.isEmpty() )
      mCategoryPath->setText( str );
    });


  setLayout( grid );
  }




void SdDOptionsPagePath::accept()
  {
  //Using SvDir convert user entered path to slash ended path
  SvDir dir;
  dir.set( mHomePath->text() );
  sdEnvir->mHomePath = dir.slashedPath();

  dir.set( mLibraryPath->text() );
  sdEnvir->mLibraryPath = dir.slashedPath();

  dir.set( mPatternPath->text() );
  sdEnvir->mPatternPath = dir.slashedPath();

  dir.set( mCategoryPath->text() );
  sdEnvir->mCategoryPath = dir.slashedPath();

  }
