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


/*!
   \brief The SdCsChannel class Object for low level information transfer

   Main idea - block transfer. One piece of interchange is block transfer.
   Block may be big size. So each block may be devided onto some packets
   and transfer continuously. On received side all packets accumulate together
   and transfer completed only when full block transfered.
 */
class SdCsChannel : public QObject
  {
    Q_OBJECT

  protected:
    QTcpSocket     *mSocket;     //! Socket, witch connection made with
    int             mReadOffset; //! Position of next received data portion
    int             mReadSize;   //! Full read size
    SdCsPacketInfo  mPacketInfo; //! Packet info of receiving block
    QByteArray      mBlock;      //! Receiving block
  public:
    explicit SdCsChannel( QTcpSocket *socket, QObject *parent = nullptr);

    /*!
       \brief onBlockReceived Block received
       \param cmd Command for block
       \param is Data stream for reading block data

       This function calling on completing block reaciving. In inherited classes
       in this function must be block data decoding.
     */
    virtual void onBlockReceived( int cmd, QDataStream &is );

    /*!
       \brief writeBlock Write block with appropriate command
       \param cmd Command for block
       \param ar Data block

       Block is completed part of interchange data. Each block follow with
       block command which descripting block data. Before block transfer it
       prepare with SdCsPacketInfo struct which contains block command and
       block size follows block data.
     */
    void         writeBlock( int cmd, QByteArray ar );

  public slots:
    /*!
       \brief onReceivBytes On receiv bytes

       Calling from socket when some data ready for read. With this function
       collecting all packets for receiving block.
     */
    void         onReceivBytes();

  };

#endif // SDCSCHANNEL_H
