/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/
#include "SdCsServer.h"
#include "SdCsChannel.h"
#include "SdCsChannelServer.h"
#include "../library/SdLibraryStorage.h"
#include "SdCsAuthorTable.h"

#include <QDebug>

SdCsServer::SdCsServer(QObject *parent) :
  QTcpServer( parent )
  {

  connect( this, &SdCsServer::newConnection, this, [this] () {
    //On every new connection we create SvChannel-s while present pending connections
    while( hasPendingConnections() )
      new SdCsChannelServer( nextPendingConnection() );
    });

  connect( &mTimer, &QTimer::timeout, this, [] () {
    //qDebug() << "timer";
    sdLibraryStorage.flush();
    sdCsAuthorTable.save();
    });

  //Set server to listen for connections
  listen( QHostAddress::Any, 1970 );

  mTimer.setInterval( 2000 );
  mTimer.start();

  qDebug() << "SaliCadServer created";
  }
