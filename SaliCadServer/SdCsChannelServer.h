/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Server implementation of channel
*/
#ifndef SDCSCHANNELSERVER_H
#define SDCSCHANNELSERVER_H

#include "SdCsChannel.h"


class SdCsChannelServer : public SdCsChannel
  {
    Q_OBJECT

  public:
    explicit SdCsChannelServer( QTcpSocket *socket, QObject *parent = nullptr );

    // SdCsChannel interface
  public:
    virtual void blockReceived() override;

  private:
    void cmGetServerVersion();
    void cmGetUpgradeList();
    void cmGetObject();
    void cmObject();
  };

#endif // SDCSCHANNELSERVER_H
