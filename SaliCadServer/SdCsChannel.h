/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Low level block transfer beatween cad server and client
*/
#ifndef SDCSCHANNEL_H
#define SDCSCHANNEL_H

#include "SdCsPacketInfo.h"

#include <QObject>
#include <QTcpSocket>
#include <QThread>
#include <QByteArray>

class SdCsChannel : public QObject
  {
    Q_OBJECT

  protected:
    QTcpSocket     *mSocket;     //Socket, witch connection made with
    int             mReadOffset; //Position of next received data portion
    int             mReadSize;   //Full read size
    SdCsPacketInfo  mPacketInfo; //Packet info of receiving block
    QByteArray      mBlock;      //Receiving block
  public:
    explicit SdCsChannel( QTcpSocket *socket, QObject *parent = nullptr);

    //Block received
    virtual void onBlockReceived( int cmd, QDataStream &is );

    //Write block
    void         writeBlock( int cmd, QByteArray ar );

  public slots:
    //On receiv bytes
    void         onReceivBytes();

  };

#endif // SDCSCHANNEL_H
