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
  mSocket(socket), //Socket, witch connection made with
  mThread(nullptr) //Thread, where SvChannel live in
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

void SdCsChannel::onReceivBytes()
  {

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
  //mSocket = nullptr;
  }
