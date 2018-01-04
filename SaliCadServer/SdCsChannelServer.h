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

    SdCadServerVersion mVersion;
    //QString            mAuthor;  //Author name receiv when user is login or register
  public:
    explicit SdCsChannelServer( QTcpSocket *socket, QObject *parent = nullptr );
    ~SdCsChannelServer();

    // SdCsChannel interface
  public:
    virtual void onBlockReceived(int cmd, QDataStream &is ) override;

  public slots:
    //On connection timeout
    void         onTimeout();

  private:
    //Register new user and add for it new machine
    void cmRegistrationRequest( QDataStream &is );
    void cmMachineRequest( QDataStream &is );
    void cmSyncRequest( QDataStream &is );
    void cmObjectRequest( QDataStream &is );


    //Add machine for user
    QString addMachine( const QString author );

    //Check user and machine key
    bool    checkAuthorAndKey( const QString author, const QString key );
  };

#endif // SDCSCHANNELSERVER_H
