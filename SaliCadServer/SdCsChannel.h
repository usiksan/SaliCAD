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

    QTcpSocket     *mSocket;     //Socket, witch connection made with
    QThread        *mThread;     //Thread, where SvChannel live in
    SdCsPacketInfo  mPacketInfo; //Packet info of receiving block
    QByteArray      mBlock;      //Receiving block
    int             mReadOffset; //Position of next received data portion
    int             mReadSize;   //Full read size
  public:
    explicit SdCsChannel( QTcpSocket *socket, QObject *parent = nullptr);

    //Block received
    virtual void blockReceived();

  public slots:
    //Write block
    void         writeBlock( int cmd, QByteArray ar );

    //On receiv bytes
    void         onReceivBytes();

    //On write complete
    virtual void onWriteComplete();

    //On connection complete
    void         onConnectionClose();
  };

#endif // SDCSCHANNEL_H
