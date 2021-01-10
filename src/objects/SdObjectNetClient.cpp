/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Client for dataBase server communicate

  1. Communicate to server
  2. Login or registrate
  3. Send request
  4. Receiv ansver
  5. Disconnect
*/
#include "SdConfig.h"
#include "SdObjectNetClient.h"
#include "library/SdLibraryStorage.h"

#include <QThread>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QHttpPart>
#include <QHttpMultiPart>
#include <QHostAddress>
#include <QSettings>
#include <QJsonDocument>

#define REPO_FIELD_AUTHOR   QStringLiteral("author")
#define REPO_FIELD_PASSWORD QStringLiteral("password")
#define REPO_FIELD_EMAIL    QStringLiteral("email")
#define REPO_FIELD_RESULT   QStringLiteral("result")
#define REPO_FIELD_INDEX    QStringLiteral("index")

//Main object for remote database communication
SdObjectNetClient *sdObjectNetClient;


static void sdHttpMultiPartAppendField( QHttpMultiPart *multiPart, const QString fieldName, const QByteArray data )
  {
  QHttpPart textPart;
  textPart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("text/plain"));
  textPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant( QStringLiteral("form-data; name=\"%1\"").arg(fieldName)) );
  textPart.setBody(data);

  multiPart->append( textPart );
  }


SdObjectNetClient::SdObjectNetClient(QObject *parent) :
  QObject( parent ),
  mQueryType(SdRemoteQueryNone),
  mCommand(0),
  mCommandSync(0)
  {
  QSettings s;
//  mAuthor          = s.value( SDK_GLOBAL_AUTHOR ).toString();
//  mKey             = s.value( SDK_MACHINE_KEY ).toString().toULongLong( nullptr, 32 );
//  mLocalSyncIndex  = s.value( SDK_LOCAL_SYNC ).toInt();
//  mRemoteSyncIndex = s.value( SDK_REMOTE_SYNC ).toInt();
//  mHostIp          = s.value( SDK_SERVER_IP ).toString();

  mTimer.setInterval( 10000 );
  qDebug() << "registered" << isRegistered();
  if( isRegistered() )
    mTimer.start();

  mNetworkManager = new QNetworkAccessManager(this);

  connect( mNetworkManager, &QNetworkAccessManager::finished, this, &SdObjectNetClient::finished );
  connect( &mTimer, &QTimer::timeout, this, &SdObjectNetClient::doSync );
  }






bool SdObjectNetClient::isRegistered() const
  {
  QSettings s;
  QString  author          = s.value( SDK_GLOBAL_AUTHOR ).toString();
  QString  passw           = s.value( SDK_GLOBAL_PASSWORD ).toString();
  QString  hostRepo        = s.value( SDK_SERVER_REPO ).toString();
  return !hostRepo.isEmpty() && !author.isEmpty() && !passw.isEmpty();
  }








//Begin registration process
void SdObjectNetClient::doRegister(const QString repo, const QString authorName, const QString password, const QString email)
  {
  mTimer.stop();

  QString fullRepo;
  if( repo.endsWith(QChar('/')) )
    fullRepo = repo;
  else
    fullRepo = repo + QStringLiteral("/");

  QSettings s;
  s.setValue( SDK_SERVER_REPO, fullRepo );

  //Prepare block for transmition
  QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

  sdHttpMultiPartAppendField( multiPart, REPO_FIELD_AUTHOR, authorName.toUtf8() );
  sdHttpMultiPartAppendField( multiPart, REPO_FIELD_PASSWORD, password.toUtf8() );
  sdHttpMultiPartAppendField( multiPart, REPO_FIELD_EMAIL, email.toUtf8() );

  emit registerStatus( tr("Register start..."), email );
  mQueryType = SdRemoteQueryRegister;
  QNetworkReply *reply = mNetworkManager->post( QNetworkRequest(QUrl( QStringLiteral("http://") + fullRepo + QStringLiteral("register.php"))), multiPart );
  multiPart->setParent(reply); // delete the multiPart with the reply
  }









//Begin object receiving process
void SdObjectNetClient::doObject(const QString hashId)
  {
//  emit remoteStatus( SdRemoteSync );
//  mCommand = 0;
//  mBuffer.clear();
//  QDataStream os( &mBuffer, QIODevice::WriteOnly );
//  QSettings s;
//  QString author          = s.value( SDK_GLOBAL_AUTHOR ).toString();
//  quint64 key             = s.value( SDK_MACHINE_KEY ).toString().toULongLong( nullptr, 32 );
//  QString hostIp          = s.value( SDK_SERVER_IP ).toString();

//  SdAuthorInfo info( author, key, 0 );
//  os << info << hashId;
//  mCommand = SCPI_OBJECT_REQUEST;
//  if( mSocket->state() != QAbstractSocket::ConnectedState ) {
//    mSocket->connectToHost( QHostAddress(hostIp), SD_DEFAULT_PORT );
//    emit process( tr("Try connect to host %1").arg(hostIp), false );
//    }
  }









//When connected to host send to it prepared block
void SdObjectNetClient::onConnected()
  {
  //qDebug() << "Connected to host" << mHostIp;
//  QSettings s;
//  QString hostIp = s.value( SDK_SERVER_IP ).toString();
//  auto msg = tr("Connected to host %1").arg(hostIp);
//  emit process( msg, false );
//  emit connectionStatus( msg, true );
//  infoAppend( msg );
//  startTransmit();
  }









//By timer do syncronisation
void SdObjectNetClient::doSync()
  {
  mTimer.setInterval( 10000 );
//  if( mQueryObjects.count() ) {
//    //There are queried objects
//    doObject( mQueryObjects.first() );
//    mQueryObjects.removeFirst();
//    return;
//    }
  QSettings s;
  QString author          = s.value( SDK_GLOBAL_AUTHOR ).toString();
  QString password        = s.value( SDK_GLOBAL_PASSWORD ).toString();
  int     localSyncIndex  = s.value( SDK_LOCAL_SYNC ).toInt();
  int     remoteSyncIndex = s.value( SDK_REMOTE_SYNC ).toInt();
  QString hostRepo        = s.value( SDK_SERVER_REPO ).toString();
  if( !hostRepo.isEmpty() && !author.isEmpty() && !password.isEmpty() ) {
    remoteStatus( SdRemoteSync );
    qDebug() << "doSync localSyncIndex" << localSyncIndex << "remoteSyncIndex" << remoteSyncIndex;
    infoAppend( tr("Do sync...") );
    //Prepare block for transmition
    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

    sdHttpMultiPartAppendField( multiPart, REPO_FIELD_AUTHOR, author.toUtf8() );
    sdHttpMultiPartAppendField( multiPart, REPO_FIELD_PASSWORD, password.toUtf8() );
    sdHttpMultiPartAppendField( multiPart, REPO_FIELD_INDEX, QByteArray::number(remoteSyncIndex) );

    mQueryType = SdRemoteQueryList;
    QNetworkReply *reply = mNetworkManager->post( QNetworkRequest(QUrl( QStringLiteral("http://") + hostRepo + QStringLiteral("list.php"))), multiPart );
    multiPart->setParent(reply); // delete the multiPart with the reply
    }
//    mCommandSync = 0;
//    mBufferSync.clear();
//    QDataStream os( &mBufferSync, QIODevice::WriteOnly );
//    SdAuthorInfo info( author, key, remoteSyncIndex );
//    os << info;

//    //Scan object list for last entered
//    QStringList list = sdLibraryStorage.getAfter( localSyncIndex );
//    //For each object write header and object itself
//    for( const QString &hash : list ) {
//      SdLibraryHeader hdr;
//      if( sdLibraryStorage.header( hash, hdr ) ) {
//        //Header readed successfull, transmit header and object
//        os << hdr << sdLibraryStorage.object(hash);
//        infoAppend( tr("Send: ") + hdr.mName );
//        }
//      }
//    qDebug() << "sync objects transmited" << list.count();
//    mLocalSyncCount = list.count();
//    mCommandSync = SCPI_SYNC_REQUEST;
//    if( mSocket->state() != QAbstractSocket::ConnectedState ) {
//      mSocket->connectToHost( QHostAddress(hostIp), SD_DEFAULT_PORT );
//      emit process( tr("Try connect to host %1").arg(hostIp), false );
//      }
//    }
  }





void SdObjectNetClient::startSync(bool start)
  {
  if( start ) {
    if( isRegistered() )
      mTimer.start();
    }
  else
    mTimer.stop();
  }








//Receiv file from repository
void SdObjectNetClient::doFile(const QString fileName)
  {
  emit remoteStatus( SdRemoteSync );
//  QSettings s;
//  QString author          = s.value( SDK_GLOBAL_AUTHOR ).toString();
//  quint64 key             = s.value( SDK_MACHINE_KEY ).toString().toULongLong( nullptr, 32 );
//  //  mLocalSyncIndex  = s.value( SDK_LOCAL_SYNC ).toInt();
//  //  mRemoteSyncIndex = s.value( SDK_REMOTE_SYNC ).toInt();
//  QString hostIp          = s.value( SDK_SERVER_IP ).toString();

//  mBuffer.clear();
//  QDataStream os( &mBuffer, QIODevice::WriteOnly );
//  SdAuthorInfo info( author, key, 0 );
//  os << info << fileName;
//  mCommand = SCPI_FILE_REQUEST;
//  if( mSocket->state() != QAbstractSocket::ConnectedState ) {
//    mSocket->connectToHost( QHostAddress(hostIp), SD_DEFAULT_PORT );
//    emit process( tr("Try connect to host %1").arg(hostIp), false );
//    }
  }







//!
//! \brief finished Called when network reply finished
//! \param reply    Reply witch being finished
//!
void SdObjectNetClient::finished(QNetworkReply *reply)
  {
  if( reply->error() ) {
    //Error when transfer
    switch( mQueryType ) {
      case SdRemoteQueryRegister :
        cmRegister( QJsonObject{} );
        emit registerStatus( tr("Error when transfer: \"%1\"").arg(reply->errorString()), QString() );
        break;
      case SdRemoteQueryList :
      case SdRemoteQueryNone :
      case SdRemoteQueryOurObjects :
      case SdRemoteQueryRepoObjects :
      case SdRemoteQueryObject :
        emit informationAppended( tr("Error when transfer: \"%1\"").arg(reply->errorString()) );
        break;
      }
    }
  else {
    //Transfer completed successfully
    //Inside reply - JSON with answer
    QJsonObject obj = QJsonDocument::fromJson( reply->readAll() ).object();
    switch( mQueryType ) {
      case SdRemoteQueryRegister :
        cmRegister( obj );
        break;
      case SdRemoteQueryList :
      case SdRemoteQueryNone :
      case SdRemoteQueryOurObjects :
      case SdRemoteQueryRepoObjects :
      case SdRemoteQueryObject :
        break;
      }
    }
  mQueryType = SdRemoteQueryNone;
  reply->deleteLater();
  }



void SdObjectNetClient::cmRegister(const QJsonObject &reply)
  {
  bool success = false;
  // 0 - успешное завершение
  // 1 - проблема со входными данными
  // 2 - не может подключиться к базе данных
  // 3 - не смог вставить новое имя в базу
  // 4 - имя уже есть в базе и пароль не совпал
  if( reply.contains(REPO_FIELD_RESULT) ) {
    int     result = reply.value( REPO_FIELD_RESULT ).toInt();
    QString email  = reply.value( REPO_FIELD_EMAIL ).toString();
    switch( result ) {
      case 0 :
        success = true;
        emit registerStatus( tr("Registration successfully"), email );
        break;
      case 1 :
        emit registerStatus( tr("Internal data. Check repo address"), email );
        break;
      case 2 :
        emit registerStatus( tr("Failure data base connection. Try later"), email );
        break;
      case 3 :
        emit registerStatus( tr("Can't insert this name into data base. Try another name"), email );
        break;
      case 4 :
        emit registerStatus( tr("This name already present in data base with different password. Try another name or restore password."), email );
        break;
      default:
        emit registerStatus( tr("Unresponsible code %1. Check repo address").arg(result), email );
        break;
      }
    }
  else {
    emit registerStatus( tr("Unresponsible reply. Check repo address"), QString{} );
    }

  if( !success ) {
    QSettings s;
    s.remove( QStringLiteral(SDK_SERVER_REPO) );
    s.remove( QStringLiteral(SDK_GLOBAL_AUTHOR) );
    s.remove( QStringLiteral(SDK_GLOBAL_PASSWORD) );
    }
  }
















void SdObjectNetClient::cmSyncList(const QJsonObject &reply)
  {
  mObjectList = QJsonArray{};
  mObjectIndex = 0;
  for( auto it = reply.constBegin(); it != reply.constEnd(); it++ ) {
    QJsonObject obj = it.value().toObject();
    obj.insert( QStringLiteral("gindex"), it.key().toInt() );
    mObjectList.append( obj );
    }
  doDownloadNextObject();
  }






void SdObjectNetClient::cmObject(QDataStream &is)
  {
//  SdAuthorInfo info;
//  is >> info;
//  qDebug() << "cmObject" << info.mAuthor << info.mKey << info.mRemain;
//  if( info.isSuccessfull() ) {
//    emit remoteStatus( SdRemoteOn );

//    //For object header
//    SdLibraryHeader header;
//    //For object store
//    QByteArray obj;

//    is >> header >> obj;

//    sdLibraryStorage.insert( header, obj );

//    infoAppend( tr("Object received: ") + header.mName );

//    QSettings s;
//    s.setValue( SDK_REMOTE_REMAIN, QString::number(info.mRemain) );

//    if( mQueryObjects.count() ) {
//      //There are queried objects
//      doObject( mQueryObjects.first() );
//      mQueryObjects.removeFirst();
//      }

//    }
//  else {
//    emit process( error(info.result()), false );
//    emit remoteStatus( SdRemoteOff );
//    }
//  emit objectComplete( info.result(), info.mRemain );
  }



void SdObjectNetClient::startTransmit()
  {
//  if( mCommandSync ) {
//    emit process( tr("Syncronisation request"), false );
//    writeBlock( mCommandSync, mBufferSync );
//    mCommandSync = 0;
//    }
//  else if( mCommand ) {
//    QString msg;
//    switch( mCommand ) {
//      case SCPI_REGISTARTION_REQUEST : msg = tr("Registration request"); break;
//      case SCPI_MACHINE_REQUEST : msg = tr("Machine appendion request"); break;
//      case SCPI_OBJECT_REQUEST : msg = tr("Object request"); break;
//      }
//    emit process( msg, false );
//    writeBlock( mCommand, mBuffer );
//    mCommand = 0;
//    }
//  else {
//    qDebug() << "Disconnect";
//    mSocket->disconnectFromHost();
//    }
  }







void SdObjectNetClient::cmFile(QDataStream &is)
  {
//  emit remoteStatus( SdRemoteOn );

//  SdAuthorInfo info;
//  QString fileName;
//  QByteArray data;
//  is >> info >> fileName >> data;
  //  emit fileContents( info.result(), fileName, data );
  }

void SdObjectNetClient::doDownloadNextObject()
  {
  if( mObjectIndex < mObjectList.count() ) {

    }
  else {
    //Complete with download objects from remote repository
    //Begin to upload newest local objects to remote repository
    }
  }




//Append info to info list
void SdObjectNetClient::infoAppend(const QString info)
  {
  //Support count of info items in info list not more then 300 items
  if( mInfoList.count() >= 300 )
    mInfoList.removeFirst();

  mInfoList.append( info );

  emit informationAppended( info );
  }














