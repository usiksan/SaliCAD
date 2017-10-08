/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  SdCsServer is tcp server. It will receiv all connect requests and
  create link channel. Link channel is connection element with client.
*/
#ifndef SDCSSERVER_H
#define SDCSSERVER_H

#include <QTcpServer>

class SdCsServer : public QTcpServer
  {
    Q_OBJECT
  public:
    explicit SdCsServer( QObject *parent = 0 );
  };

#endif // SDCSSERVER_H
