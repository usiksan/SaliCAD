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
#include "../objects/SdUtil.h"


#include <QDir>
#include <QFile>
#include <QCoreApplication>
#include <QDebug>
#include <QUuid>
#include <QTimer>

SdLibraryStorage sdLibraryStorage;

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
  sdCsAuthorTable.registerAuthor( info.mAuthor, info.mEmail, 5, info.mRemain, &info.mKey );

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

  SdItemInfoList list;
  SdCategoryInfoList categoryList;
  SdTranslationInfoList translationList;

  if( sdCsAuthorTable.login( info.mAuthor, info.mKey ) ) {
    //Execute sync
    is >> categoryList >> translationList;


    for( const SdCategoryInfo &info : categoryList ) {
      //Replace object
      QSqlQuery q;
      q.prepare( QString("SELECT * FROM hierarchy WHERE section='%1' AND time>%2").arg( info.mSection ).arg( info.mTime ) );
      q.exec();
      if( q.first() ) continue;

      q.exec( QString("DELETE FROM hierarchy WHERE section='%1' AND time<=%2").arg( info.mSection ).arg( info.mTime ) );

      //Insert record with new object
      q.prepare( QString("INSERT INTO hierarchy (section, path, parent, time) "
                                   "VALUES (:section, :path, :parent, :time)") );
      q.bindValue( QStringLiteral(":section"), info.mSection );
      q.bindValue( QStringLiteral(":path"), info.mPath );
      q.bindValue( QStringLiteral(":parent"), info.mParent );
      q.bindValue( QStringLiteral(":time"), info.mTime );
      q.exec();
      }


    for( const SdTranslationInfo &info : translationList ) {
      //Replace object
      QSqlQuery q;
      q.prepare( QString("SELECT * FROM translation WHERE section='%1' AND lang='%2' AND time>%3").arg( info.mSection ).arg( info.mLang ).arg( info.mTime ) );
      q.exec();
      if( q.first() ) continue;

      q.exec( QString("DELETE FROM translation WHERE section='%1' AND lang='%2' AND time<=%3").arg( info.mSection ).arg( info.mLang ).arg( info.mTime ) );

      //Insert record with new object
      q.prepare( QString("INSERT INTO hierarchy (translate, lang, section, time) "
                                   "VALUES (:translate, :lang, :section, :time)") );
      q.bindValue( QStringLiteral(":translate"), info.mTranslate );
      q.bindValue( QStringLiteral(":lang"), info.mLang );
      q.bindValue( QStringLiteral(":section"), info.mSection );
      q.bindValue( QStringLiteral(":time"), info.mTime );
      q.exec();
      }

    //Append objects if presend
    bool good = true;
    while( !is.atEnd() ) {
      //Object
      SdItemInfo info;
      QByteArray obj;
      //Read object
      is >> info >> obj;
      //Replace object
      QSqlQuery q;
      q.prepare( QString("SELECT * FROM objects WHERE name='%1' AND author='%2'").arg( info.mName ).arg( info.mAuthor ) );
      q.exec();
      if( q.first() ) {
        //Check if in database newest object or not
        if( q.record().field( QStringLiteral("timeCreate") ).value().toInt() > info.mTimeCreate )
          //In database newest object do nothing
          continue;

        //Delete record
        q.prepare( QString("DELETE FROM objects WHERE name='%1' AND author='%2'").arg( info.mName ).arg( info.mAuthor ) );
        q.exec();
        }
      //Insert record with new object
      q.prepare( QString("INSERT INTO objects (hash, name, author, tag, timeCreate, class, timeUpgrade, object) "
                                   "VALUES (:hash, :name, :author, :tag, :timeCreate, :class, :timeUpgrade, :object)") );
      q.bindValue( QStringLiteral(":hash"), info.mHashId );
      q.bindValue( QStringLiteral(":name"), info.mName );
      q.bindValue( QStringLiteral(":author"), info.mAuthor );
      q.bindValue( QStringLiteral(":tag"), info.mTag );
      q.bindValue( QStringLiteral(":timeCreate"), info.mTimeCreate );
      q.bindValue( QStringLiteral(":class"), info.mObjectClass );
      q.bindValue( QStringLiteral(":timeUpgrade"), SdUtil::getTime2000() );
      q.bindValue( QStringLiteral(":object"), QVariant( obj ), QSql::Binary | QSql::In );
      if( !q.exec() ) {
        good = false;
        qDebug() << "Unable insert new object" << q.lastError();
        }
      }


    //Query to select all object after append time
    QSqlQuery q;
    q.prepare( QString("SELECT * FROM objects WHERE timeUpgrade>%1").arg( info.mLastSync ) );
    q.exec();
    if( q.first() ) {
      do {
        //Object info to send
        SdItemInfo item;
        //Fill info from object record
        FillItemInfo( item, q.record() );
        //Append info to list
        list.append( item );
        }
      while( q.next() );
      }

    categoryList.clear();
    //Query to select all category after append time
    q.prepare( QString("SELECT * FROM hierarchy WHERE time>%1").arg( info.mLastSync ) );
    q.exec();
    if( q.first() ) {
      do {
        SdCategoryInfo item;
        //Fill item from object record
        item.mSection = q.record().field( QStringLiteral("section") ).value().toString();
        item.mParent  = q.record().field( QStringLiteral("parent") ).value().toString();
        item.mPath    = q.record().field( QStringLiteral("path") ).value().toString();
        item.mTime    = q.record().field( QStringLiteral("time") ).value().toInt();
        categoryList.append( item );
        }
      while( q.next() );
      }

    translationList.clear();
    //Query to select all category after append time
    q.prepare( QString("SELECT * FROM translation WHERE time>%1").arg( info.mLastSync ) );
    q.exec();
    if( q.first() ) {
      do {
        SdTranslationInfo item;
        //Fill item from object record
        item.mSection   = q.record().field( QStringLiteral("section") ).value().toString();
        item.mLang      = q.record().field( QStringLiteral("lang") ).value().toString();
        item.mTranslate = q.record().field( QStringLiteral("translate") ).value().toString();
        item.mTime      = q.record().field( QStringLiteral("time") ).value().toInt();
        translationList.append( item );
        }
      while( q.next() );
      }

    if( good ) {
      info.mLastSync = SdUtil::getTime2000();
      info.mResult = SCPE_SUCCESSFULL;
      }
    else
      info.mResult = SCPE_SYNC_FAIL;
    }
  else
    //Login fail
    info.mResult = SCPE_NOT_REGISTERED;

  //Prepare block
  QByteArray ar;
  QDataStream os( &ar, QIODevice::WriteOnly );
  os << mVersion << info << categoryList << translationList << list;

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
    info.mKey = 0;

  os << mVersion << info;
  header.write( os );
  os << obj;

  //Transmit object
  writeBlock( SCPI_OBJECT, ar );
  }





