/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Remote library interchange status
*/
#include "SdWRemoteStatus.h"
#include "objects/SdObjectNetClient.h"

SdWRemoteStatus *SdWRemoteStatus::mWRemoteStatus;

SdWRemoteStatus::SdWRemoteStatus(QWidget *parent) :
  QListWidget(parent)
  {
  mWRemoteStatus = this;

  setWindowFlag( Qt::Dialog );
  setWindowFlag( Qt::WindowStaysOnTopHint );

  setWindowTitle( tr("Remote link messages") );

  //Add existings info items
  addItems( sdObjectNetClient->infoList() );

  //Track to end of list
  setCurrentRow( count() - 1 );

  //When appended new info item
  connect( sdObjectNetClient, &SdObjectNetClient::informationAppended, this, [this] ( const QString info ) {
    //Append info to visual list
    addItem( info );

    //Track to end of list
    setCurrentRow( count() - 1 );
    } );

  connect( &mHideTimer, &QTimer::timeout, this, &SdWRemoteStatus::close );
  mHideTimer.start( 15000 );

  connect( this, &SdWRemoteStatus::currentItemChanged, this, &SdWRemoteStatus::restartTimer );
  connect( this, &SdWRemoteStatus::currentRowChanged, this, &SdWRemoteStatus::restartTimer );
  }



SdWRemoteStatus::~SdWRemoteStatus()
  {
  mWRemoteStatus = nullptr;
  }




void SdWRemoteStatus::restartTimer()
  {
  mHideTimer.start( 15000 );
  }



void SdWRemoteStatus::closeEvent(QCloseEvent *event)
  {
  QListWidget::closeEvent(event);
  deleteLater();
  }
