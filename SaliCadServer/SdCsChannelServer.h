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

    QString mAuthor; //Author name receiv when user is login or register
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
    void cmLogin();

    //Register new user and add for it new machine
    void    cmRegister();

    //Add machine for user
    QString addMachine( const QString author );
  };

#endif // SDCSCHANNELSERVER_H
