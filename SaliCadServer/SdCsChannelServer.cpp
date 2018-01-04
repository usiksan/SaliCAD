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
#include <QTimer>


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

  static QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
  //Test if previous dbase is opened and close it if nessesery
  if( !db.isOpen() ) {

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
      if( !query.exec("CREATE TABLE objects (hash TEXT PRIMARY KEY, "
                      " name TEXT, author TEXT, tag TEXT, timeCreate INTEGER, class INTEGER, timeUpgrade INTEGER, object BLOB)") )
        qDebug() << "Fail create objects table";

      //Create hierarchical table
      if( !query.exec("CREATE TABLE hierarchy (section TEXT PRIMARY KEY, path TEXT, parent TEXT, time INTEGER)") )
        qDebug() << "Fail create hierarchy table";
      //Hierarchy table translation on all languages
      if( !query.exec("CREATE TABLE translation (translate TEXT PRIMARY KEY, lang TEXT, section TEXT, time INTEGER)") )
        qDebug() << "Fail create translation table";




      //User table
      //Every user have
      // description - any author description
      // limit count - element count he can download within their licension plan
      // delivered count - element count witch already downloaded
      if( !query.exec("CREATE TABLE users (author TEXT PRIMARY KEY, description TEXT, delivered INTEGER, maxLimit INTEGER)") )
        qDebug() << "Fail create users table" << query.lastError();

      //Every user can have many machine on witch he works
      //this table contains this machines list
      if( !query.exec("CREATE TABLE machines (uid TEXT PRIMARY KEY, author TEXT, changeTime INTEGER)") )
        qDebug() << "Fail create machines table";
      }
    }
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

  //qDebug() << "cmRegistrationRequest" << info.mAuthor;

  if( info.mAuthor.isEmpty() )
    info.mResult = SCPE_AUTHOR_IS_EMPTY;
  else {
    //Query if author already present
    QSqlQuery q;
    q.exec( QString("SELECT * FROM users WHERE author='%1'").arg(info.mAuthor) );
    if( q.first() )
      //Author already present
      info.mResult = SCPE_AUTHOR_ALREADY_PRESENT;
    else {
      info.mDelivered = 0;
      //No author with this name, register
      q.clear();
      q.prepare( QString("INSERT INTO users (author, description, delivered, maxLimit) "
                         "VALUES (:author, :description, :delivered, :maxLimit)") );
      q.bindValue( QStringLiteral(":author"), info.mAuthor );
      q.bindValue( QStringLiteral(":description"), info.mDescription );
      q.bindValue( QStringLiteral(":delivered"), info.mDelivered );
      q.bindValue( QStringLiteral(":maxLimit"), info.mLimit );
      if( q.exec() ) {
        //Author added. Insert first machine for new user.
        info.mKey = addMachine( info.mAuthor );
        info.mResult = SCPE_SUCCESSFULL;
        }
      else {
        qDebug() << q.lastError();
        info.mResult = SCPE_REGISTER_FAIL;
        }
      }
    }

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

  //Query if author already present
  QSqlQuery q;
  q.exec( QString("SELECT * FROM users WHERE author='%1'").arg(info.mAuthor) );
  if( q.first() ) {
    //Author already present, fill fields
    info.mLimit       = q.record().field(QStringLiteral("maxLimit")).value().toInt();
    info.mDelivered   = q.record().field(QStringLiteral("delivered")).value().toInt();
    info.mDescription = q.record().field(QStringLiteral("description")).value().toString();
    //Check if machine present
    q.exec( QString("SELECT * FROM machines WHERE uid='%1' AND author='%2'").arg(info.mKey).arg(info.mAuthor) );
    if( q.first() ) {
      //Key present, append machine
      info.mKey = addMachine( info.mAuthor );
      if( info.mKey.isEmpty() )
        info.mResult = SCPE_MACHINE_LIMIT;
      else
        info.mResult = SCPE_SUCCESSFULL;
      }
    else
      //Invalid key for this author
      info.mResult = SCPE_INVALID_KEY;
    }
  else
    //Author not found
    info.mResult = SCPE_NOT_REGISTERED;

  //Send answer
  QByteArray ar;
  QDataStream os( &ar, QIODevice::WriteOnly );
  os << mVersion << info;
  writeBlock( SCPI_MACHINE_INFO, ar );
  }






void FillItemInfo( SdItemInfo &info, QSqlRecord rec ) {
  info.mHashId      = rec.field( QString("hash") ).value().toString();
  info.mName        = rec.field( QString("name") ).value().toString();
  info.mAuthor      = rec.field( QString("author") ).value().toString();
  info.mObjectClass = rec.field( QString("class") ).value().toLongLong();
  info.mTimeCreate  = rec.field( QString("timeCreate") ).value().toUInt();
  }





void SdCsChannelServer::cmSyncRequest(QDataStream &is)
  {
  SdAuthorInfo info;
  is >> info;

  SdItemInfoList list;
  SdCategoryInfoList categoryList;
  SdTranslationInfoList translationList;

  if( checkAuthorAndKey( info.mAuthor, info.mKey ) ) {
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

  //For object store
  SdItemInfo item;
  QByteArray obj;

  if( checkAuthorAndKey( info.mAuthor, info.mKey ) ) {
    //Execute request objects
    QString hash;

    is >> hash;

    //Query to select object with requested hash
    QSqlQuery q;
    q.prepare( QString("SELECT * FROM objects WHERE hash='%1'").arg( hash ) );
    q.exec();
    if( q.first() ) {
      //Object is present

      //Fill info from object record
      FillItemInfo( item, q.record() );

      obj = q.record().field( QString("object") ).value().toByteArray();

      //Increment component count
      q.exec( QString("SELECT * FROM users WHERE author='%1'").arg(info.mAuthor) );
      if( q.first() ) {
        //Author already present, fill fields
        info.mLimit       = q.record().field(QStringLiteral("maxLimit")).value().toInt();
        info.mDelivered   = q.record().field(QStringLiteral("delivered")).value().toInt();
        if( info.mDelivered < info.mLimit ) {
          info.mDelivered++;
          q.exec( QString("UPDATE users SET delivered=%1 WHERE author='%2'").arg(info.mDelivered).arg(info.mAuthor));
          info.mResult = SCPE_SUCCESSFULL;
          }
        else
          info.mResult = SCPE_OBJECT_LIMIT;
        }
      else
        info.mResult = SCPE_NOT_REGISTERED;
      }
    else
      info.mResult = SCPE_OBJECT_NOT_FOUND;
    }
  else
    //Login fail
    info.mResult = SCPE_NOT_REGISTERED;

  os << mVersion << info << item << obj;

  //Transmit object
  writeBlock( SCPI_OBJECT, ar );
  }









//Add machine for user
QString SdCsChannelServer::addMachine(const QString author)
  {
  //Prepare key for machine
  QString uid = QUuid::createUuid().toString();
  //Prepare change time when key will changed
  int changeTime = SdUtil::getTime2000() + 200000;

  QSqlQuery q;
  //Get registered machines count
  q.prepare( QString("SELECT * FROM machines WHERE author='%1'").arg( author ) );
  q.exec();
  int count = 0;
  if( q.first() ) {
    do { count++; } while( q.next() );
    }
  qDebug() << "limit" << count;
  if( count > MACHINE_LIMIT ) {
    qDebug() << "addMachine limit" << q.size();
    return QString();
    }

  //Prepare query to database
  q.prepare( QString("INSERT INTO machines (uid, author, changeTime) "
                     "VALUES (:uid, :author, :changeTime)") );
  q.bindValue( QStringLiteral(":uid"), uid );
  q.bindValue( QStringLiteral(":author"), author );
  q.bindValue( QStringLiteral(":changeTime"), changeTime );
  if( q.exec() )
    //Machine registerd successfully, return its key
    return uid;
  qDebug() << "addMachine" << q.lastError();
  //Wrong registration
  return QString();
  }




//Check user and machine key
bool SdCsChannelServer::checkAuthorAndKey(const QString author, const QString key)
  {
  //Query if author already present
  QSqlQuery q;
  q.exec( QString("SELECT * FROM machines WHERE uid='%1' AND author='%2'").arg(key).arg(author) );
  if( q.first() )
    //Key present
    return true;
  return false;
  }
