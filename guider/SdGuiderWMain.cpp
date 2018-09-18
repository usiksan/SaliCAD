/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/
#include "SdGuiderWMain.h"

#include <QMenuBar>
#include <QSettings>
#include <QFileInfo>
#include <QComboBox>
#include <QFileDialog>
#include <QSplitter>
#include <QDebug>


SdGuiderWMain::SdGuiderWMain(QWidget *parent) :
  QMainWindow(parent)
  {
  QMenu *menuFile = new QMenu( tr("File") );
  menuFile->addAction( tr("Open..."), this, &SdGuiderWMain::cmFileOpen );
  menuFile->addAction( tr("Save"), this, &SdGuiderWMain::cmFileSave );

  QMenu *menuPlay = new QMenu( tr("Play") );
  menuPlay->addAction( tr("Restart"), this, &SdGuiderWMain::cmPlayRestart );
  menuPlay->addAction( tr("Start"), this, &SdGuiderWMain::cmPlayStart );
  menuPlay->addAction( tr("Stop"), this, &SdGuiderWMain::cmPlayStop );

  QMenuBar *bar = menuBar();
  bar->addMenu( menuFile );
  bar->addMenu( menuPlay );

  QSplitter *splitter = new QSplitter;
  mView = new QLabel;
  mTimeList = new QListWidget;
  splitter->addWidget(mTimeList);
  splitter->addWidget(mView);
  setCentralWidget( splitter );
  }

void SdGuiderWMain::cmFileOpen()
  {
  mFileName = QFileDialog::getOpenFileName( this, tr("Select movie for open") );
  if( !mFileName.isEmpty() ) {
    mFile.load( mFileName );
    mFile.play(0);
    mView->setPixmap( QPixmap::fromImage( mFile.mBack ) );
    }
  }

void SdGuiderWMain::cmFileSave()
  {

  }

void SdGuiderWMain::cmPlayRestart()
  {

  }

void SdGuiderWMain::cmPlayStart()
  {

  }

void SdGuiderWMain::cmPlayStop()
  {

  }

void SdGuiderWMain::play()
  {

  }
