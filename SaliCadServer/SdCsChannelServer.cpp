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

  //New author registration
  if( !sdCsAuthorTable.registerAuthor( info.mAuthor, info.mEmail, 5, info.mRemain, &info.mKey ) )
    info.setFail();

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

  if( sdCsAuthorTable.registerMachine( info.mAuthor, &info.mKey ) )
    info.mRemain = sdCsAuthorTable.remainObject( info.mAuthor );
  else info.setFail();

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

  if( sdCsAuthorTable.login( info.mAuthor, info.mKey ) ) {
    //Execute sync
    //Prepare objects with newer index
    QStringList newerObjects = sdLibraryStorage.getAfter( info.mSyncIndex );
    QStringList newerCategories = sdLibraryStorage.categoryGetAfter( info.mSyncIndex );
    info.mSyncIndex += newerObjects.count() + newerCategories.count();
    os << info;

    //Setup received categories
    SdCategoryInfoList categoryList;
    is >> categoryList;


    for( const SdCategoryInfo &info : categoryList )
      //Replace category
      sdLibraryStorage.categoryInsert( info.mCategory, info.mAssociation );


    //Append objects if presend
    while( !is.atEnd() ) {
      //Object
      SdLibraryHeader header;
      QByteArray      obj;
      //Read object
      is >> header >> obj;
      //Replace object
      QString hash = header.id();

      if( !sdLibraryStorage.contains(hash) ) {
        //Insert new object
        sdLibraryStorage.insert( hash, header, obj );
        }
      }


    //At now prepare categories with founded interval
    categoryList.clear();
    for( const QString &str : newerCategories ) {
      SdCategoryInfo cinf;
      cinf.mCategory = str;
      cinf.mAssociation = sdLibraryStorage.category( str );
      categoryList.append( cinf );
      }

    os << categoryList;

    //At now write object headers
    for( const QString &hash : newerObjects ) {
      SdLibraryHeader hdr;
      sdLibraryStorage.header( hash, hdr );
      os << hdr;
      }
    }
  else {
    info.setFail();
    os << info;
    }

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
    QString hash;

    is >> hash;

    //Query to select object with requested hash
    obj = sdLibraryStorage.object(hash);
    sdLibraryStorage.header( hash, header );
    }
  else
    //Login fail
    info.setFail();

  os << mVersion << info << header << obj;

  //Transmit object
  writeBlock( SCPI_OBJECT, ar );
  }





