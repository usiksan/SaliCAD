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
#include "SdGuiderDTiterEdit.h"

#include <QMenuBar>
#include <QSettings>
#include <QFileInfo>
#include <QComboBox>
#include <QFileDialog>
#include <QSplitter>
#include <QDebug>
#include <QMessageBox>
#include <QInputDialog>
#include <QTextToSpeech>
#include <QVBoxLayout>

static QTextToSpeech *speech;

SdGuiderWMain::SdGuiderWMain(QWidget *parent) :
  QMainWindow(parent),
  mLock(false)
  {
  speech = new QTextToSpeech();
  mLanguage = QStringLiteral("en");
  mFile.mTiterChanged = [this] () { titerChanged(); };

  QMenu *menuFile = new QMenu( tr("File") );
  menuFile->addAction( tr("Open..."), this, &SdGuiderWMain::cmFileOpen );
  menuFile->addAction( tr("Save"), this, &SdGuiderWMain::cmFileSave );


  QMenu *menuTiter = new QMenu( tr("Titer") );
  menuTiter->addAction( tr("Insert"), this, &SdGuiderWMain::cmTiterInsert );
  menuTiter->addAction( tr("Hide"), this, &SdGuiderWMain::cmTiterHide );
  menuTiter->addAction( tr("Edit"), this, &SdGuiderWMain::cmTiterEdit );
  menuTiter->addAction( tr("Copy"), this, &SdGuiderWMain::cmTiterCopy );
  menuTiter->addAction( tr("Cut"), this, &SdGuiderWMain::cmTiterCut );
  menuTiter->addAction( tr("Paste"), this, &SdGuiderWMain::cmTiterPaste );
  menuTiter->addAction( tr("Language"), this, &SdGuiderWMain::cmTiterLanguage );


  QMenu *menuPlay = new QMenu( tr("Play") );
  menuPlay->addAction( tr("Restart"), this, &SdGuiderWMain::cmPlayRestart );
  menuPlay->addAction( tr("Start"), this, &SdGuiderWMain::cmPlayStart );
  menuPlay->addAction( tr("Stop"), this, &SdGuiderWMain::cmPlayStop );

  QMenuBar *bar = menuBar();
  bar->addMenu( menuFile );
  bar->addMenu( menuTiter );
  bar->addMenu( menuPlay );

  QSplitter *splitter = new QSplitter;

  //Time list
  mTimeList = new QListWidget;
  splitter->addWidget(mTimeList);

  //View and titer table
  QWidget *w = new QWidget();
  QVBoxLayout *lay = new QVBoxLayout();
  mView = new QLabel;
  mView->setMinimumHeight( 700 );
  lay->addWidget( mView );
  mTiterTable = new QTableWidget();
  lay->addWidget( mTiterTable );
  lay->setMargin(0);
  w->setLayout( lay );

  splitter->addWidget(w);

  setCentralWidget( splitter );

  connect( &mTimer, &QTimer::timeout, this, &SdGuiderWMain::play );
  connect( mTimeList, &QListWidget::currentRowChanged, this, &SdGuiderWMain::onRowChanged );
  }




void SdGuiderWMain::cmFileOpen()
  {
  mFileName = QFileDialog::getOpenFileName( this, tr("Select movie for open") );
  if( !mFileName.isEmpty() ) {
    mFile.load( mFileName );
    mFile.play(0);
    mLock = true;
    mTimeList->clear();
    for( int i = 0; i < mFile.mFile.count(); i++ )
      mTimeList->addItem( mFile.mFile.at(i).title() );
    mCurrentTime = 0;
    mTimeList->setCurrentRow(0);
    mLock = false;
    play();
    }
  }





void SdGuiderWMain::cmFileSave()
  {
  if( !mFileName.isEmpty() )
    mFile.save( mFileName );
  }




void SdGuiderWMain::cmTiterEdit()
  {
  int row = mTimeList->currentRow();
  if( row < 0 ) row = 0;
  if( row < mFile.mFile.count() ) {
    if( mFile.mFile.at(row).mType == SD_GT_TITER ) {
      SdGuiderTiter titer;
      titer.read( mFile.mFile.at(row).mData );
      //Edit dialog
      SdGuiderDTiterEdit edit( &titer, this );
      if( edit.exec() )
        mFile.mFile[row].mData = titer.write();
      }
    else QMessageBox::warning( this, tr("Warning!"), tr("This item is not titer") );
    }
  }




void SdGuiderWMain::cmTiterInsert()
  {
  int row = mTimeList->currentRow();
  if( row < 0 ) row = 0;
  if( row < mFile.mFile.count() ) {
    int time = mFile.mFile.at(row).mTime;
    bool ok = true;
    time = QInputDialog::getInt( this, tr("Titer insert"), tr("Enter time to insert titer"), time, 0, 600000, 1, &ok );
    if( ok ) {
      QString text = QInputDialog::getText( this, tr("Titer insert"), tr("Enter english titer") );
      SdGuiderTiter titer;
      titer.mContens.insert( QString("en"), text );
      SdGuiderTime tm;
      tm.mType = SD_GT_TITER;
      tm.mTime = time;
      tm.mData = titer.write();
      mFile.mFile.insert( row, tm );
      refreshTime();
      }
    }
  }




void SdGuiderWMain::cmTiterHide()
  {
  int row = mTimeList->currentRow();
  if( row < 0 ) row = 0;
  if( row < mFile.mFile.count() ) {
    int time = mFile.mFile.at(row).mTime;
    bool ok = true;
    time = QInputDialog::getInt( this, tr("Titer insert"), tr("Enter time to insert titer hide"), time, 0, 600000, 1, &ok );
    if( ok ) {
      SdGuiderTime tm;
      tm.mType = SD_GT_TITER_HIDE;
      tm.mTime = time;
      mFile.mFile.insert( row, tm );
      refreshTime();
      }
    }
  }



static SdGuiderTiter localTiter;

void SdGuiderWMain::cmTiterCopy()
  {
  int row = mTimeList->currentRow();
  if( row < 0 ) row = 0;
  if( row < mFile.mFile.count() ) {
    if( mFile.mFile.at(row).mType == SD_GT_TITER )
      localTiter.read( mFile.mFile.at(row).mData );
    else QMessageBox::warning( this, tr("Warning!"), tr("This item is not titer") );
    }
  }





void SdGuiderWMain::cmTiterCut()
  {
  int row = mTimeList->currentRow();
  if( row < 0 ) row = 0;
  if( row < mFile.mFile.count() ) {
    if( mFile.mFile.at(row).mType == SD_GT_TITER ) {
      localTiter.read( mFile.mFile.at(row).mData );
      mFile.mFile.removeAt(row);
      refreshTime();
      }
    else QMessageBox::warning( this, tr("Warning!"), tr("This item is not titer") );
    }
  }




void SdGuiderWMain::cmTiterPaste()
  {
  int row = mTimeList->currentRow();
  if( row < 0 ) row = 0;
  if( row < mFile.mFile.count() ) {
    int time = mFile.mFile.at(row).mTime;
    bool ok = true;
    time = QInputDialog::getInt( this, tr("Titer insert"), tr("Enter time to paste titer"), time, 0, 600000, 1, &ok );
    if( ok ) {
      SdGuiderTime tm;
      tm.mType = SD_GT_TITER;
      tm.mTime = time;
      tm.mData = localTiter.write();
      mFile.mFile.insert( row, tm );
      refreshTime();
      }
    }
  }




void SdGuiderWMain::cmTiterLanguage()
  {
  mLanguage = QInputDialog::getText( this, tr("Titer insert"), tr("Enter english titer"), QLineEdit::Normal, mLanguage );
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
  mLock = true;
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
  mLock = false;
  }




void SdGuiderWMain::onRowChanged(int row)
  {
  //qDebug() << row;
  //return;
  if( !mLock && row >= 0 && row < mFile.mFile.count() ) {
    mCurrentTime = mFile.mFile.at(row).mTime;
    int lim = (mCurrentTime / 100) * 100;
    //Find key frame
    int i;
    for( i = row; i > 0; i-- )
      if( mFile.mFile.at(i).mTime < lim ) break;

    while( i <= row )
      mFile.play(i++);

    mView->clear();
    mView->setPixmap( mFile.build() );
    }
  }



void SdGuiderWMain::onCellChanged(int row, int column)
  {
  if( mFile.mTiterIndex >= 0 ) {
    if( row == mFile.mTiter.mContens.count() && column == 0 ) {
      //Appended new empty row
      QString key = mTiterTable->item( row, 0 )->text();
      mFile.mTiter.mContens.insert( key, mTiterTable->item( row, 1 )->text() );
      disconnect( mTiterTable, &QTableWidget::cellChanged, this, &SdGuiderWMain::onCellChanged );
      mTiterTable->item( row, 0 )->setData( Qt::UserRole, key );
      row++;
      mTiterTable->insertRow( row );
      mTiterTable->setRowHeight( row, 25 );
      mTiterTable->setItem( row, 0, new QTableWidgetItem() );
      mTiterTable->setItem( row, 1, new QTableWidgetItem() );
      connect( mTiterTable, &QTableWidget::cellChanged, this, &SdGuiderWMain::onCellChanged );
      }
    else {
      if( column == 0 ) {
        //Key changed
        QString key = mTiterTable->item( row, 0 )->data( Qt::UserRole ).toString();
        QString newKey = mTiterTable->item( row, 0 )->text();
        if( newKey.isEmpty() ) {
          //Delete record
          mTiterTable->removeRow( row );
          mFile.mTiter.mContens.remove( key );
          }
        else {
          //Replace record
          mFile.mTiter.mContens.remove( key );
          mFile.mTiter.mContens.insert( newKey, mTiterTable->item(row,1)->text() );
          mTiterTable->item( row, 0 )->setData( Qt::UserRole, newKey );
          }
        }
      else {
        //Set data
        QString key = mTiterTable->item( row, 0 )->text();
        QString val = mTiterTable->item( row, 1 )->text();
        mFile.mTiter.mContens.insert( key, val );
        }
      }
    //Update titer
    mFile.updateTiter();
    }
  }




void SdGuiderWMain::refreshTime()
  {
  mLock = true;
  int row = mTimeList->currentRow();
  mTimeList->clear();
  for( int i = 0; i < mFile.mFile.count(); i++ )
    mTimeList->addItem( mFile.mFile.at(i).title() );
  mCurrentTime = 0;
  mTimeList->setCurrentRow(0);
  mTimeList->setCurrentRow(row);
  mLock = false;
  onRowChanged(row);
  }




void SdGuiderWMain::titerChanged()
  {
  disconnect( mTiterTable, &QTableWidget::cellChanged, this, &SdGuiderWMain::onCellChanged );
  mTiterTable->clear();
  if( mFile.mTiterIndex >= 0 ) {
    mTiterTable->setColumnCount(2);
    mTiterTable->setRowCount(mFile.mTiter.mContens.count() + 1);
    mTiterTable->setHorizontalHeaderLabels( {tr("Language"), tr("Titer contents") } );
    mTiterTable->setColumnWidth( 0, 120 );
    mTiterTable->setColumnWidth( 1, 600 );
    int row = 0;
    for( auto iter = mFile.mTiter.mContens.cbegin(); iter != mFile.mTiter.mContens.cend(); iter++ ) {
      mTiterTable->setRowHeight( row, 25 );
      QTableWidgetItem *item = new QTableWidgetItem(iter.key());
      item->setData( Qt::UserRole, iter.key() );
      mTiterTable->setItem( row, 0, item );
      mTiterTable->setItem( row, 1, new QTableWidgetItem(iter.value()) );
      row++;
      }
    mTiterTable->setRowHeight( row, 25 );
    mTiterTable->setItem( row, 0, new QTableWidgetItem() );
    mTiterTable->setItem( row, 1, new QTableWidgetItem() );

    if( !mLanguage.isEmpty() ) {
      speech->say( mFile.mTiter.mContens.value( mLanguage ) );
      }
    }
  connect( mTiterTable, &QTableWidget::cellChanged, this, &SdGuiderWMain::onCellChanged );
  }



