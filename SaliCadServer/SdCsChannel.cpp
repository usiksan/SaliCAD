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

SdCsChannel::SdCsChannel(QTcpSocket *socket, QObject *parent) :
  QObject(parent),
  mSocket(socket),  //Socket, witch connection made with
  mThread(nullptr), //Thread, where SvChannel live in
  mReadOffset(0),   //Position of next received data portion
  mReadSize(0)      //Full read size
  {

  //Create thread, where SvChannel will be live in
  mThread = new QThread();
  moveToThread( mThread );

  //Connections
  connect( mSocket, &QTcpSocket::readyRead, this, &SdCsChannel::onReceivBytes );
  connect( mSocket, &QTcpSocket::bytesWritten, this, &SdCsChannel::onWriteComplete );
  connect( mSocket, &QTcpSocket::disconnected, this, &SdCsChannel::onConnectionClose );

  mThread->start();
  }




//Block received
void SdCsChannel::blockReceived()
  {

  }




void SdCsChannel::writeBlock(int cmd, QByteArray ar)
  {
  //Prepare block header info
  SdCsPacketInfo info( cmd, ar.size() );
  //Write block header
  mSocket->write( (const char*)(&info), sizeof(SdCsPacketInfo) );
  //If block is nonzero then write block contens
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
      if( mReadOffset >= mReadSize )
        blockReceived();
      }
    else {
      //Packet info received
      if( mSocket->read( (char*) (&mPacketInfo), sizeof(SdCsPacketInfo) ) != sizeof(SdCsPacketInfo) ) {
        //Sycronization error
        }
      else {
        mReadSize = mPacketInfo.lenght();
        if( mReadSize ) {
          mReadOffset = 0;
          mBlock.resize( mReadSize );
          }
        else {
          //No addon data requered
          blockReceived();
          }
        }
      }
    }
  }




void SdCsChannel::onWriteComplete()
  {

  }





void SdCsChannel::onConnectionClose()
  {
  mThread->quit();
  mThread->deleteLater();
  deleteLater();
  mSocket->deleteLater();
  }
