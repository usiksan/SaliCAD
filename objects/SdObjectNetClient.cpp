/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/
#include "SdConfig.h"
#include "SdObjectNetClient.h"
#include "SaliCadServer/SdCsPacketInfo.h"

#include <QThread>
#include <QHostAddress>
#include <QSettings>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlField>
#include <QSqlError>


//Main object for remote database communication
SdObjectNetClient *sdObjectNetClient;


SdObjectNetClient::SdObjectNetClient(QObject *parent) :
  SdCsChannel( new QTcpSocket(), parent ),
  mCommand(0),
  mCommandSync(0)
  {
  QSettings s;
  mAuthorInfo.mAuthor   = s.value( SDK_GLOBAL_AUTHOR ).toString();
  mAuthorInfo.mKey      = s.value( SDK_MACHINE_KEY ).toString();
  mAuthorInfo.mLastSync = s.value( SDK_LAST_SYNC ).toInt();
  mHostIp               = s.value( SDK_SERVER_IP ).toString();

  mTimer.setInterval( 30000 );
  mTimer.start();

  connect( mSocket, &QTcpSocket::connected, this, &SdObjectNetClient::onConnected );
  connect( &mTimer, &QTimer::timeout, this, &SdObjectNetClient::doSync );
  }






bool SdObjectNetClient::isRegistered() const
  {
  return !mHostIp.isEmpty() && !mAuthorInfo.mAuthor.isEmpty() && !mAuthorInfo.mKey.isEmpty();
  }








//Begin registration process
void SdObjectNetClient::doRegistration(const QString ip, const QString authorName, const QString description)
  {
  //Prepare block for transmition
  mHostIp                  = ip;
  mAuthorInfo.mAuthor      = authorName;
  mAuthorInfo.mDescription = description;
  mAuthorInfo.mLimit       = 100;
  mAuthorInfo.mDelivered   = 0;
  mAuthorInfo.mResult      = SCPE_UNDEFINED;
  mAuthorInfo.mLastSync    = 0;
  mBuffer.clear();
  QDataStream os( &mBuffer, QIODevice::WriteOnly );
  os << mAuthorInfo;
  mCommand = SCPI_REGISTARTION_REQUEST;
  if( mSocket->state() != QAbstractSocket::ConnectedState )
    mSocket->connectToHost( QHostAddress(mHostIp), SD_DEFAULT_PORT );
  }





//Begin append machine
void SdObjectNetClient::doMachine(const QString ip, const QString authorName, const QString key)
  {
  //Prepare block for transmition
  mHostIp                  = ip;
  mAuthorInfo.mAuthor      = authorName;
  mAuthorInfo.mKey         = key;
  mAuthorInfo.mLimit       = 100;
  mAuthorInfo.mDelivered   = 0;
  mAuthorInfo.mResult      = SCPE_UNDEFINED;
  mAuthorInfo.mLastSync    = 0;
  mBuffer.clear();
  QDataStream os( &mBuffer, QIODevice::WriteOnly );
  os << mAuthorInfo;
  mCommand = SCPI_MACHINE_REQUEST;
  if( mSocket->state() != QAbstractSocket::ConnectedState )
    mSocket->connectToHost( QHostAddress(mHostIp), SD_DEFAULT_PORT );
  }





//Begin object receiving process
void SdObjectNetClient::doObject(const QString hashId)
  {
  mCommand = 0;
  mBuffer.clear();
  QDataStream os( &mBuffer, QIODevice::WriteOnly );
  os << mAuthorInfo << hashId;
  mCommand = SCPI_OBJECT_REQUEST;
  if( mSocket->state() != QAbstractSocket::ConnectedState )
    mSocket->connectToHost( QHostAddress(mHostIp), SD_DEFAULT_PORT );
  }









//When connected to host send to it prepared block
void SdObjectNetClient::onConnected()
  {
  qDebug() << "Connected to host" << mHostIp;
  startTransmit();
  }




void FillItemInfo( SdItemInfo &info, QSqlRecord rec ) {
  info.mHashId      = rec.field( QString("hash") ).value().toString();
  info.mName        = rec.field( QString("name") ).value().toString();
  info.mAuthor      = rec.field( QString("author") ).value().toString();
  info.mTag         = rec.field( QString("tag") ).value().toString();
  info.mObjectClass = rec.field( QString("class") ).value().toLongLong();
  info.mTimeCreate  = rec.field( QString("timeCreate") ).value().toUInt();
  }







//By timer do syncronisation
void SdObjectNetClient::doSync()
  {
  if( isRegistered() ) {
    qDebug() << "doSync";
    mCommandSync = 0;
    mBufferSync.clear();
    QDataStream os( &mBufferSync, QIODevice::WriteOnly );
    os << mAuthorInfo;

    SdCategoryInfoList categoryList;
    SdTranslationInfoList translationList;
    QSqlQuery q;

    categoryList.clear();
    //Query to select all category after append time
    q.prepare( QString("SELECT * FROM hierarchy WHERE time>%1").arg( mAuthorInfo.mLastSync ) );
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
    q.prepare( QString("SELECT * FROM translation WHERE time>%1").arg( mAuthorInfo.mLastSync ) );
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

    os << categoryList << translationList;
    qDebug() << "sync category" << categoryList.count();
    qDebug() << "sync translation" << translationList.count();

    int objCount = 0;
    //Query to select all object after append time
    q.prepare( QString("SELECT * FROM objects WHERE timeCreate>'%1'").arg( mAuthorInfo.mLastSync ) );
    q.exec();
    if( q.first() ) {
      do {
        //Object info to send
        SdItemInfo item;
        //Fill info from object record
        FillItemInfo( item, q.record() );
        //Append info to stream
        os << item;
        QByteArray obj = q.record().field("object").value().toByteArray();
        os << obj;
        objCount++;
        }
      while( q.next() );
      }
    qDebug() << "sync obj" << objCount;

    mCommandSync = SCPI_SYNC_REQUEST;
    if( mSocket->state() != QAbstractSocket::ConnectedState )
      mSocket->connectToHost( QHostAddress(mHostIp), SD_DEFAULT_PORT );
    }
  }










void SdObjectNetClient::onBlockReceived(int cmd, QDataStream &is)
  {
  SdCadServerVersion version;
  is >> version >> mAuthorInfo;
  switch( cmd ) {
    case SCPI_REGISTRATION_INFO :
      cmRegistrationInfo( is );
      break;

    case SCPI_MACHINE_INFO :
      cmRegistrationInfo( is );
      break;

    case SCPI_SYNC_LIST :
      cmSyncList( is );
      break;

    case SCPI_OBJECT :
      cmObject( is );
      break;
    }
  startTransmit();
  }




void SdObjectNetClient::cmRegistrationInfo(QDataStream &is)
  {
  Q_UNUSED(is)
  qDebug() << "cmRegistrationInfo" << mAuthorInfo.mKey;
  if( mAuthorInfo.mResult == SCPE_SUCCESSFULL ) {
    QSettings s;
    s.setValue( SDK_GLOBAL_AUTHOR, mAuthorInfo.mAuthor );
    s.setValue( SDK_MACHINE_KEY, mAuthorInfo.mKey );
    s.setValue( SDK_LAST_SYNC, mAuthorInfo.mLastSync );
    s.setValue( SDK_SERVER_IP, mHostIp );
    }
  emit registrationComplete( mAuthorInfo.mAuthor, mAuthorInfo.mDescription, mAuthorInfo.mKey, mAuthorInfo.mLimit, mAuthorInfo.mDelivered, mAuthorInfo.mResult );
  }




void SdObjectNetClient::cmSyncList(QDataStream &is)
  {
  qDebug() << "cmSyncList" << mAuthorInfo.mResult;
  if( mAuthorInfo.mResult == SCPE_SUCCESSFULL ) {
    SdCategoryInfoList categoryList;
    SdTranslationInfoList translationList;
    SdItemInfoList list;
    is >> categoryList >> translationList >> list;

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

    for( const SdItemInfo &info : list ) {
      QSqlQuery q;
      q.prepare( QString("SELECT * FROM objects WHERE hash='%1'").arg( info.mHashId ) );
      q.exec();
      if( q.first() )
        //Object already present in local database
        continue;

      //Add upgraded object
      q.prepare( QString("INSERT INTO objects (hash, name, author, tag, timeCreate, class) "
                           "VALUES (:hash, :name, :author, :tag, :timeCreate, :class)") );
      q.bindValue( QStringLiteral(":hash"), info.mHashId );
      q.bindValue( QStringLiteral(":name"), info.mName );
      q.bindValue( QStringLiteral(":author"), info.mAuthor );
      q.bindValue( QStringLiteral(":tag"), info.mTag );
      q.bindValue( QStringLiteral(":timeCreate"), info.mTimeCreate );
      q.bindValue( QStringLiteral(":class"), info.mObjectClass );
      q.exec();
      }
    }
  }




void SdObjectNetClient::cmObject(QDataStream &is)
  {
  qDebug() << "cmObject" << mAuthorInfo.mResult;
  if( mAuthorInfo.mResult == SCPE_SUCCESSFULL ) {
    SdItemInfo info;
    QByteArray obj;
    is >> info >> obj;

    QSqlQuery q;
    q.prepare( QString("DELETE FROM objects WHERE hash='%1'").arg( info.mHashId ) );
    q.exec();

    //Add upgraded object
    q.prepare( QString("INSERT INTO objects (hash, name, author, tag, timeCreate, class, object) "
                         "VALUES (:hash, :name, :author, :tag, :timeCreate, :class, :object)") );
    q.bindValue( QStringLiteral(":hash"), info.mHashId );
    q.bindValue( QStringLiteral(":name"), info.mName );
    q.bindValue( QStringLiteral(":author"), info.mAuthor );
    q.bindValue( QStringLiteral(":tag"), info.mTag );
    q.bindValue( QStringLiteral(":timeCreate"), info.mTimeCreate );
    q.bindValue( QStringLiteral(":class"), info.mObjectClass );
    q.bindValue( QStringLiteral(":object"), QVariant( obj ), QSql::Binary | QSql::In );
    q.exec();
    }
  emit objectComplete( mAuthorInfo.mResult );
  }



void SdObjectNetClient::startTransmit()
  {
  if( mCommandSync ) {
    writeBlock( mCommandSync, mBufferSync );
    mCommandSync = 0;
    }
  else if( mCommand ) {
    writeBlock( mCommand, mBuffer );
    mCommand = 0;
    }
  else {
    qDebug() << "Disconnect";
    mSocket->disconnectFromHost();
    }
  }






