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
    QString            mAuthor;  //Author name receiv when user is login or register
  public:
    explicit SdCsChannelServer( QTcpSocket *socket, QObject *parent = nullptr );
    ~SdCsChannelServer() override;

    // SdCsChannel interface
  public:
    virtual void onBlockReceived(int cmd, QDataStream &is ) override;

  public slots:
    //On connection timeout
    void         onTimeout();

  private:
    //Register new user and add for it new machine
    void cmRegistrationRequest( QDataStream &is );

    //Append new machine to existing user
    void cmMachineRequest( QDataStream &is );

    //Syncronisation request. Objects from local to remote and header list from remote to local
    void cmSyncRequest( QDataStream &is );

    //Object request. Local requests object by uid. Remote return object if available
    void cmObjectRequest( QDataStream &is );

    //Object removing. Local requests deleting object by uid.
    void cmDeleteRequest( QDataStream &is );

    //File request. Local requests file by fileName. Remote return file if available.
    //All files must be located in upload directory
    void cmFileRequest( QDataStream &is );

    //Check registration
    void cmAccessCheckRequest( QDataStream &is );
  };

#endif // SDCSCHANNELSERVER_H
