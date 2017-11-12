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
#include "../objects/SdUtil.h"


#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlField>
#include <QSqlError>
#include <QDir>
#include <QFile>
#include <QCoreApplication>
#include <QDebug>
#include <QUuid>


SdCsChannelServer::SdCsChannelServer(QTcpSocket *socket, QObject *parent) :
  SdCsChannel( socket, parent )
  {
  static QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
  //Test if previous dbase is opened and close it if nessesery
  if( db.isOpen() )
    db.close();

  //Test if database exist
  bool presence = QFile::exists( QCoreApplication::applicationDirPath() + QString( LIBRARY_PATH LIBRARY_FILE ) );
  //Create directory if nessesery
  QDir().mkpath( QCoreApplication::applicationDirPath() + QString( LIBRARY_PATH ) );
  db.setDatabaseName( QCoreApplication::applicationDirPath() + QString( LIBRARY_PATH LIBRARY_FILE ) );
  if( !db.open() ) {
    qDebug() << "Cannot open database";
    qDebug() << "Unable to establish a database connection.\n"
                "This example needs SQLite support. Please read "
                "the Qt SQL driver documentation for information how "
                "to build it.";
    return;
    }

  if( !presence ) {
    QSqlQuery query;
    //Create objects table
    query.exec("CREATE TABLE objects (hash TEXT PRIMARY KEY, "
               " name TEXT, author TEXT, tag TEXT, timeCreate INTEGER, class INTEGER, timeUpgrade INTEGER, object BLOB)");

    //Create hierarchical table
    query.exec("CREATE TABLE hierarchy (section TEXT PRIMARY KEY, path TEXT, parent TEXT, time INTEGER)");
    //Hierarchy table translation on all languages
    query.exec("CREATE TABLE translation (translate TEXT PRIMARY KEY, lang TEXT, section TEXT, time INTEGER)");



    //User table
    //Every user have
    // description - any author description
    // limit count - element count he can download within their licension plan
    // delivered count - element count witch already downloaded
    query.exec("CREATE TABLE users (author TEXT PRIMARY KEY, description TEXT, delivered INTEGER, limit INTEGER)");

    //Every user can have many machine on witch he works
    //this table contains this machines list
    query.exec("CREATE TABLE machines (uid TEXT PRIMARY KEY, author TEXT, changeTime INTEGER)");
    }
  }




void SdCsChannelServer::blockReceived()
  {
  switch( mPacketInfo.command() ) {
    case SCPI_GET_SERVER_VERSION   :
      //Get cad server version to decide properties
      //data: none
      cmGetServerVersion();
      break;

    case SCPI_REQUEST_AUTHOR :
      //Register new user
      cmRegister();
      break;

    case SCPI_GET_UPDATE_LIST      :
      //Request to get update data base list
      //data: quint32 - time of last upgrading
      cmGetUpgradeList();
      break;

    case SCPI_GET_OBJECT           :
      //Request to get object from data base with desired hashId
      //data: QString - hashId of requested object
      cmGetObject();
      break;

    case SCPI_OBJECT               :
      //Object
      //data: SdItemInfo QByteArray (object contens)
      cmObject();
      break;

    }
  }




//Get cad server version to decide properties
//data: none
void SdCsChannelServer::cmGetServerVersion()
  {
  //Prepare version info
  SdCadServerVersion version;
  version.mMajor      = SALI_CAD_MAJOR;
  version.mMinor      = SALI_CAD_MINOR;
  version.mServerName = SALI_CAD_SERVER_INFO;

  QByteArray ar;
  version.write( ar );

  //Send
  writeBlock( SCPI_SERVER_VERSION, ar );
  }




void FillItemInfo( SdItemInfo &info, QSqlRecord rec ) {
  info.mHashId      = rec.field( QString("hash") ).value().toString();
  info.mName        = rec.field( QString("name") ).value().toString();
  info.mAuthor      = rec.field( QString("author") ).value().toString();
  info.mObjectClass = rec.field( QString("class") ).value().toInt();
  info.mTimeCreate  = rec.field( QString("timeCreate") ).value().toInt();
  }





//Request to get update data base list
//data: quint32 - time of last upgrading
void SdCsChannelServer::cmGetUpgradeList()
  {
  quint32 upgradeTime;
  QDataStream is( mBlock );
  is >> upgradeTime;

  SdItemInfoList list;

  //Query to select all object after append time
  QSqlQuery q;
  q.prepare( QString("SELECT * FROM objects WHERE timeUpgrade>'%1'").arg( upgradeTime ) );
  q.exec();
  if( q.first() ) {
    do {
      //Object info to send
      SdItemInfo info;
      //Fill info from object record
      FillItemInfo( info, q.record() );
      //Append info to list
      list.append( info );
      }
    while( q.next() );
    }

  //Prepare block
  QByteArray ar;
  if( list.count() ) {
    QDataStream os( &ar, QIODevice::WriteOnly );
    os << list;
    }

  //Send block
  writeBlock( SCPI_UPGRADE_LIST, ar );
  }





//Request to get object from data base with desired hashId
//data: QString - hashId of requested object
void SdCsChannelServer::cmGetObject()
  {
  SdItemInfo info;
  QByteArray obj;
  QString hash;
  QDataStream is( mBlock );
  is >> hash;

  //Query to select object with requested hash
  QSqlQuery q;
  q.prepare( QString("SELECT * FROM objects WHERE hash>'%1'").arg( hash ) );
  q.exec();
  if( q.first() ) {
    //Object is present

    //Fill info from object record
    FillItemInfo( info, q.record() );

    obj = q.record().field( QString("object") ).value().toByteArray();
    }

  //Prepare answer
  QByteArray ar;
  QDataStream os( &ar, QIODevice::WriteOnly );
  os << info << obj;

  //Transmit object
  writeBlock( SCPI_OBJECT, ar );
  }




void SdCsChannelServer::cmObject()
  {
  SdItemInfo info;
  QByteArray obj;
  QDataStream is( mBlock );
  is >> info >> obj;

  //Query to insert new object

  //First check if database already contains newest object
//  QSqlQuery q;
//  //Insert new object
//  qDebug() << "insert";
//  q.prepare( QString("INSERT INTO objects (hash, name, author, timeCreate, class, timeUpgrade, object) "
//                       "VALUES (:hash, :name, :author, :timeCreate, :class, :timeUpgrade, :object)") );
//  q.bindValue( QStringLiteral(":hash"), info.mHashId );
//  q.bindValue( QStringLiteral(":name"), info.mName );
//  q.bindValue( QStringLiteral(":author"), item->getAuthor() );
//  q.bindValue( QStringLiteral(":timeCreate"), item->getTime() );
//  q.bindValue( QStringLiteral(":class"), item->getClass() );
//  q.bindValue( QStringLiteral(":timeUpgrade"), 0 );
//  q.bindValue( QStringLiteral(":object"), QVariant( QJsonDocument(obj).toBinaryData() ), QSql::Binary | QSql::In );
//  q.exec();

  }




//Register new user
void SdCsChannelServer::cmRegister()
  {
  SdAuthorInfo info;
  QDataStream is( mBlock );
  is >> info;

  //Query if author already present
  QSqlQuery q;
  q.exec( QString("SELECT * FROM users WHERE author='%1'").arg(info.mAuthor) );
  if( q.first() ) {
    //Author already present
    info.mKey.clear();
    info.mLimit = -1;
    info.mDelivered = -1;
    }
  else {
    info.mDelivered = 0;
    //No author with this name, register
    q.prepare( QString("INSERT INTO users (author, description, delivered, limit) "
                       "VALUES (:author, :description, :delivered, :limit)") );
    q.bindValue( QStringLiteral(":author"), info.mAuthor );
    q.bindValue( QStringLiteral(":description"), info.mDescription );
    q.bindValue( QStringLiteral(":delivered"), info.mDelivered );
    q.bindValue( QStringLiteral(":limit"), info.mLimit );
    if( q.exec() ) {
      //Author added. Insert first machine for new user.
      info.mKey = addMachine( info.mAuthor );

      //If register successfull then login automaticly
      if( !info.mKey.isEmpty() )
        mAuthor = info.mAuthor;
      }
    else info.mLimit = -1;
    }

  //Prepare answer
  QByteArray ar;
  QDataStream os( &ar, QIODevice::WriteOnly );
  os << info;

  //Transmit object
  writeBlock( SCPI_ACKNOWLEDGE, ar );
  }




//Add machine for user
QString SdCsChannelServer::addMachine(const QString author)
  {
  //Prepare key for machine
  QString uid = QUuid::createUuid().toString();
  //Prepare change time when key will changed
  int changeTime = SdUtil::getTime2000() + 200000;

  //Prepare query to database
  QSqlQuery q;
  q.prepare( QString("INSERT INTO machines (uid, author, changeTime) "
                     "VALUES (:uid, :author, :changeTime)") );
  q.bindValue( QStringLiteral(":uid"), uid );
  q.bindValue( QStringLiteral(":author"), author );
  q.bindValue( QStringLiteral(":changeTime"), changeTime );
  if( q.exec() )
    //Machine registerd successfully, return its key
    return uid;
  //Wrong registration
  return QString();
  }
