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

#include <QDebug>

SdCsServer::SdCsServer(QObject *parent) :
  QTcpServer( parent )
  {

  connect( this, &SdCsServer::newConnection, this, [this] () {
    //On every new connection we create SvChannel-s while present pending connections
    while( hasPendingConnections() )
      new SdCsChannelServer( nextPendingConnection() );
    });

  //Set server to listen for connections
  listen( QHostAddress::Any, 1970 );

  qDebug() << "SaliCadServer created";
  }
