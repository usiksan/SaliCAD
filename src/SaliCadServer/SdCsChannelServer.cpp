/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/
#include "SdCsConfig.h"
#include "SdCsChannelServer.h"
#include "SdCsAuthorTable.h"
#include "../library/SdLibraryStorage.h"


#include <QDir>
#include <QFile>
#include <QCoreApplication>
#include <QDebug>
#include <QUuid>
#include <QTimer>

SdLibraryStorage sdLibraryStorage;
SdCsAuthorTable  sdCsAuthorTable;

SdCsChannelServer::SdCsChannelServer(QTcpSocket *socket, QObject *parent) :
  SdCsChannel( socket, parent )
  {
  qDebug() << "Server channel open";
  connect( mSocket, &QTcpSocket::disconnected, this, &SdCsChannelServer::deleteLater );
  connect( mSocket, &QTcpSocket::disconnected, mSocket, &QTcpSocket::deleteLater );
  QTimer::singleShot( 10000, this, &SdCsChannelServer::onTimeout );

  //Prepare version info
  mVersion.mMajor      = SALI_CAD_MAJOR;
  mVersion.mMinor      = SALI_CAD_MINOR;
  mVersion.mServerName = SALI_CAD_SERVER_INFO;
  }





SdCsChannelServer::~SdCsChannelServer()
  {
  qDebug() << "Channel is closed";
  }





void SdCsChannelServer::onBlockReceived( int cmd, QDataStream &is)
  {
  //qDebug() << "onBlockReceived" << cmd;
  switch( cmd ) {
    case SCPI_REGISTARTION_REQUEST :
      cmRegistrationRequest( is );
      break;

    case SCPI_MACHINE_REQUEST :
      cmMachineRequest( is );
      break;

    case SCPI_SYNC_REQUEST :
      cmSyncRequest( is );
      break;

    case SCPI_OBJECT_REQUEST :
      cmObjectRequest( is );
      break;

    default :
      mSocket->disconnectFromHost();
    }
  }



//On connection timeout
void SdCsChannelServer::onTimeout()
  {
  qDebug() << "Timeout disconnected";
  mSocket->disconnectFromHost();
  }





//Register new user
void SdCsChannelServer::cmRegistrationRequest(QDataStream &is)
  {
  SdAuthorInfo info;
  is >> info;

  if( info.mAuthor.isEmpty() )
    info.setFail( SCPE_AUTHOR_IS_EMPTY );

  //New author registration
  else if( !sdCsAuthorTable.registerAuthor( info.mAuthor, info.mEmail, 5, info.mRemain, &info.mKey ) )
    info.setFail( SCPE_AUTHOR_ALREADY_PRESENT );

  else
    //All ok
    info.setResult( SCPE_SUCCESSFULL );

  qDebug() << "User registration " << info.mAuthor;
  //Send answer
  QByteArray ar;
  QDataStream os( &ar, QIODevice::WriteOnly );
  os << mVersion << info;
  writeBlock( SCPI_REGISTRATION_INFO, ar );
  }






void SdCsChannelServer::cmMachineRequest(QDataStream &is)
  {
  SdAuthorInfo info;
  is >> info;

  if( !sdCsAuthorTable.login( info.mAuthor, info.mKey ) )
    info.setFail( SCPE_NOT_REGISTERED );
  else if( sdCsAuthorTable.registerMachine( info.mAuthor, &info.mKey ) ) {
    info.mRemain = sdCsAuthorTable.remainObject( info.mAuthor );
    info.setResult( SCPE_SUCCESSFULL );
    }
  else info.setFail( SCPE_MACHINE_LIMIT );

  qDebug() << "Machine append " << info.mAuthor << info.mKey;
  //Send answer
  QByteArray ar;
  QDataStream os( &ar, QIODevice::WriteOnly );
  os << mVersion << info;
  writeBlock( SCPI_MACHINE_INFO, ar );
  }










void SdCsChannelServer::cmSyncRequest(QDataStream &is)
  {
  SdAuthorInfo info;
  is >> info;

  //Prepare answer block
  QByteArray ar;
  QDataStream os( &ar, QIODevice::WriteOnly );
  os << mVersion;

  //Prepare objects with newer index
  QStringList newerObjects = sdLibraryStorage.getAfter( info.mSyncIndex );

  if( sdCsAuthorTable.login( info.mAuthor, info.mKey ) ) {
    //Execute sync
    info.setResult( SCPE_SUCCESSFULL );

    //Append objects if presend
    while( !is.atEnd() ) {
      //Object
      SdLibraryHeader header;
      QByteArray      obj;
      //Read object
      is >> header >> obj;
      //Insert new object
      sdLibraryStorage.insert( header, obj );
      }
    }
  else
    info.setFail( SCPE_NOT_REGISTERED );

  os << info;

  //At now write object headers
  for( const QString &hash : newerObjects ) {
    SdLibraryHeader hdr;
    sdLibraryStorage.header( hash, hdr );
    os << hdr;
    }

  //qDebug() << "For author " << info.mAuthor << " headers " << newerObjects.count();

  //Send block
  writeBlock( SCPI_SYNC_LIST, ar );
  }





void SdCsChannelServer::cmObjectRequest(QDataStream &is)
  {
  SdAuthorInfo info;
  is >> info;

  //Prepare answer
  QByteArray ar;
  QDataStream os( &ar, QIODevice::WriteOnly );

  //For object header
  SdLibraryHeader header;
  //For object store
  QByteArray obj;

  if( sdCsAuthorTable.login( info.mAuthor, info.mKey ) ) {
    //Execute request objects
    QString uid;

    is >> uid;

    if( sdLibraryStorage.contains( uid ) ) {
      if( sdCsAuthorTable.remainObject( info.mAuthor ) ) {
        info.mRemain = sdCsAuthorTable.decrementObject( info.mAuthor );
        //Query to select object with requested hash
        obj = sdLibraryStorage.object(uid);
        sdLibraryStorage.header( uid, header );
        info.setResult( SCPE_SUCCESSFULL );
        }
      else {
        info.setResult( SCPE_OBJECT_LIMIT );
        info.mRemain = 0;
        }
      }
    else {
      info.setResult( SCPE_OBJECT_NOT_FOUND );
      info.mRemain = sdCsAuthorTable.remainObject( info.mAuthor );
      }
    }
  else
    //Login fail
    info.setFail( SCPE_NOT_REGISTERED );


  if( info.isSuccessfull() ) {
    os << mVersion << info << header << obj;
    qDebug() << "For author " << info.mAuthor << " object " << header.mName;
    }
  else
    os << mVersion << info;

  //Transmit object
  writeBlock( SCPI_OBJECT, ar );
  }





