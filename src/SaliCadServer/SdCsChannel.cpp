/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/
#include "SdCsChannel.h"
#include <QDataStream>

SdCsChannel::SdCsChannel(QTcpSocket *socket, QObject *parent) :
  QObject(parent),
  mSocket(socket),  //Socket, witch connection made with
  mReadOffset(0),   //Position of next received data portion
  mReadSize(0)      //Full read size
  {

  //Connections
  connect( mSocket, &QTcpSocket::readyRead, this, &SdCsChannel::onReceivBytes );
//  connect( mSocket, &QTcpSocket::bytesWritten, this, &SdCsChannel::onWriteComplete );
//  connect( mSocket, &QTcpSocket::disconnected, this, &SdCsChannel::onConnectionClose );
  }



void SdCsChannel::onBlockReceived(int cmd, QDataStream &is)
  {
  Q_UNUSED( cmd );
  Q_UNUSED( is );
  }





void SdCsChannel::writeBlock(int cmd, QByteArray ar)
  {
  //Prepare block header info
  SdCsPacketInfo info( cmd, ar.size() );
  //Write block header
  mSocket->write( (const char*)(&info), sizeof(SdCsPacketInfo) );
  //If block is nonzero then write block contents
  if( ar.size() )
    mSocket->write( ar );
  }




void SdCsChannel::onReceivBytes()
  {
  //There any data from net
  while( mSocket->bytesAvailable() ) {
    //Check if block is reading
    if( mReadOffset < mReadSize ) {
      //Block part received
      int len = mSocket->read( mBlock.data() + mReadOffset, mReadSize - mReadOffset );
      mReadOffset += len;
      //if last part of block then parse received block
      if( mReadOffset >= mReadSize ) {
        QDataStream is(mBlock);
        onBlockReceived( mPacketInfo.command(), is );
        return;
        }
      }
    else {
      //Packet info received
      if( mSocket->read( (char*) (&mPacketInfo), sizeof(SdCsPacketInfo) ) != sizeof(SdCsPacketInfo) ) {
        //Syncronization error, break connection
        mSocket->disconnectFromHost();
        return;
        }
      else {
        mReadSize = mPacketInfo.lenght();
        if( mReadSize ) {
          mReadOffset = 0;
          mBlock.resize( mReadSize );
          }
        else {
          //No addon data requered - syncronization error, break connection
          mSocket->disconnectFromHost();
          return;
          }
        }
      }
    }
  }








#if 0
void SdCsChannel::onConnectionClose()
  {
  mThread->quit();
  mThread->deleteLater();
  deleteLater();
  mSocket->deleteLater();
  }




void SdCsChannel::sendAuthorInfo(int cmd, SdAuthorInfo &info)
  {
  QByteArray ar;
  QDataStream os( &ar, QIODevice::WriteOnly );
  os << info;
  writeBlock( cmd, ar );
  }
#endif
