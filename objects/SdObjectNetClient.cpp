/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/
#include "SdConfig.h"
#include "SdObjectNetClient.h"
#include "SaliCadServer/SdCsPacketInfo.h"

#include <QThread>
#include <QHostAddress>

//Main object for remote database communication
SdObjectNetClient *sdObjectNetClient;


SdObjectNetClient::SdObjectNetClient(QObject *parent) :
  QObject(parent),
  mSocket( new QTcpSocket() )
  {
  QThread *th = new QThread();
  moveToThread( th );
  mSocket->moveToThread( th );

  connect( th, &QThread::started, this, &SdObjectNetClient::start );

  th->start();
  }




//Begin object receiving process
void SdObjectNetClient::needObject(const QString hashId)
  {

  }




//Begin registration process
void SdObjectNetClient::needRegistration( const QString ip, const QString name, const QString description)
  {
  mSocket->connectToHost( QHostAddress(ip), SD_DEFAULT_PORT );
  if( mSocket->waitForConnected() ) {
    //Prepare request packet
    SdCsPacketInfo info;
    info.

    }
  else {
    //Can't connect
    emit loginComplete( name, QString(), 0, 0 );
    }
  }




//Begin login process
void SdObjectNetClient::needLogin(const QString name, const QString key)
  {

  }



//Begin background process
void SdObjectNetClient::start()
  {
  mTimer = new QTimer();
  mTimer->setInterval( 30000 );
  }
