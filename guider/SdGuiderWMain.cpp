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

  connect( &mTimer, &QTimer::timeout, this, &SdGuiderWMain::play );
  }

void SdGuiderWMain::cmFileOpen()
  {
  mFileName = QFileDialog::getOpenFileName( this, tr("Select movie for open") );
  if( !mFileName.isEmpty() ) {
    mFile.load( mFileName );
    mFile.play(0);
    for( int i = 0; i < mFile.mFile.count(); i++ )
      mTimeList->addItem( mFile.mFile.at(i).title() );
    mCurrentTime = 0;
    mTimeList->setCurrentRow(0);
    play();
    }
  }





void SdGuiderWMain::cmFileSave()
  {
  if( !mFileName.isEmpty() )
    mFile.save( mFileName );
  }




void SdGuiderWMain::cmPlayRestart()
  {
  mCurrentTime = 0;
  mTimeList->setCurrentRow(0);
  mTimer.start(100);
  }




void SdGuiderWMain::cmPlayStart()
  {
  mTimer.start(100);
  }



void SdGuiderWMain::cmPlayStop()
  {
  mTimer.stop();
  }



void SdGuiderWMain::play()
  {
  mCurrentTime++;
  int row = mTimeList->currentRow();
  if( row < 0 ) row = 0;
  bool build = false;
  while( row < mFile.mFile.count() && mFile.mFile.at(row).mTime < mCurrentTime ) {
    mFile.play(row++);
    build = true;
    }
  if( build ) {
    mView->clear();
    mView->setPixmap( mFile.build() );
    }
  if( row >= mFile.mFile.count() )
    cmPlayStop();
  else
    mTimeList->setCurrentRow( row );
  }
