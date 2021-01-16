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
#define REPO_FIELD_UID      QStringLiteral("uid")
#define REPO_FIELD_TIME     QStringLiteral("time")
#define REPO_FIELD_OBJECT   QStringLiteral("object")

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
  mQueryType(SdRemoteQueryNone)
  {

  mTimer.setInterval( 1000 );
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









//By timer do syncronisation
void SdObjectNetClient::doSync()
  {
  mTimer.setInterval(10000);
  if( mQueryType == SdRemoteQueryNone ) {
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
    }
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
    SdRemoteQueryType queryType = mQueryType;
    mQueryType = SdRemoteQueryNone;
    switch( queryType ) {
      case SdRemoteQueryRegister :
        cmRegister( QJsonObject{} );
        emit registerStatus( tr("Error when transfer: \"%1\"").arg(reply->errorString()), QString() );
        break;
      case SdRemoteQueryList :
      case SdRemoteQueryNone :
      case SdRemoteQueryUploadObject :
      case SdRemoteQueryDownloadObject :
        emit informationAppended( tr("Error when transfer: \"%1\"").arg(reply->errorString()) );
        break;
      }
    }
  else {
    //Transfer completed successfully
    //Inside reply - JSON with answer
    QJsonObject obj = QJsonDocument::fromJson( reply->readAll() ).object();
    SdRemoteQueryType queryType = mQueryType;
    mQueryType = SdRemoteQueryNone;
    switch( queryType ) {
      case SdRemoteQueryRegister :
        cmRegister( obj );
        break;
      case SdRemoteQueryList :
        cmSyncList( obj );
        break;
      case SdRemoteQueryDownloadObject :
        cmDownloadObject( obj );
        break;
      case SdRemoteQueryUploadObject :
        cmUploadObject( obj );
        break;
      case SdRemoteQueryNone :
        break;
      }
    }
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
  if( reply.value( QStringLiteral("result") ).toInt() == 0 ) {
    //Retrive object list from reply
    QJsonObject list = reply.value( QStringLiteral("list") ).toObject();
    mObjectList = QJsonArray{};
    mObjectIndex = 0;
    qDebug() << "sync list received:" << list.count();
    infoAppend( tr("Sync list received %1 objects").arg(list.count()) );
    for( auto it = list.constBegin(); it != list.constEnd(); it++ ) {
      QJsonObject obj = it.value().toObject();
      obj.insert( QStringLiteral("gindex"), it.key().toInt() );
      mObjectList.append( obj );
      qDebug() << "[" << it.key().toInt() << "]=" << obj.value( REPO_FIELD_UID ).toString();
      }
    doDownloadNextObject();
    }
  }





void SdObjectNetClient::cmDownloadObject(const QJsonObject &reply)
  {
  if( reply.value( QStringLiteral("result") ).toInt() == 0 ) {
    //Object download successfully
    int        remoteSyncIndex = reply.value( REPO_FIELD_INDEX ).toInt();
    QByteArray objectHex       = reply.value( REPO_FIELD_OBJECT ).toString().toLatin1();
    QByteArray object = QByteArray::fromHex( objectHex );

    QDataStream is( object );
    SdLibraryHeader header;
    QByteArray objectBody;
    is >> header >> objectBody;

    qDebug() << "sync downloaded successfully [" << remoteSyncIndex << "]" << header.uid();

    sdLibraryStorage.insert( header, objectBody );

    QSettings s;
    s.setValue( SDK_REMOTE_SYNC, remoteSyncIndex );
    mObjectIndex++;
    }
  else if( reply.value( QStringLiteral("result") ).toInt() == 3 ) {
    //Запрошенный объект не найден - хрень какая-то
    mObjectIndex++;
    qDebug() << "sync download failure";
    }
  doDownloadNextObject();
  }




void SdObjectNetClient::cmUploadObject(const QJsonObject &reply)
  {
  if( reply.value( QStringLiteral("result") ).toInt() == 0 ) {
    //Object uploaded successfully
    qDebug() << "sync uploaded successfully";
    //Increment object counter
    QSettings s;
    int localSyncIndex  = s.value( SDK_LOCAL_SYNC ).toInt();
    s.setValue( SDK_LOCAL_SYNC, ++localSyncIndex );
    doUploadNextObject();
    }
  }






//void SdObjectNetClient::cmObject(QDataStream &is)
//  {
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
//  }








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
    QSettings s;
    QString author          = s.value( SDK_GLOBAL_AUTHOR ).toString();
    QString password        = s.value( SDK_GLOBAL_PASSWORD ).toString();
    int     remoteSyncIndex = s.value( SDK_REMOTE_SYNC ).toInt();
    QString hostRepo        = s.value( SDK_SERVER_REPO ).toString();
    if( !hostRepo.isEmpty() && !author.isEmpty() && !password.isEmpty() ) {
      //Check object
      QJsonObject obj = mObjectList.at( mObjectIndex ).toObject();
      QString uid    = obj.value( QStringLiteral("uid") ).toString();
      int     time   = obj.value( QStringLiteral("time") ).toInt();
      int     gindex = obj.value( QStringLiteral("gindex") ).toInt();
      if( sdLibraryStorage.isNewerOrSameObject( uid, time ) ) {
        //Object in data-base newer or same, so need not to dowload it
        remoteSyncIndex = gindex;
        s.setValue( SDK_REMOTE_SYNC, remoteSyncIndex );
        mObjectIndex++;
        doDownloadNextObject();
        }
      else {
        //We need to download object
        //Prepare block for transmition
        QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

        qDebug() << "sync query download" << "[" << gindex << "]=" << uid;
        infoAppend( tr("Download \"%d\"").arg(uid) );
        sdHttpMultiPartAppendField( multiPart, REPO_FIELD_AUTHOR, author.toUtf8() );
        sdHttpMultiPartAppendField( multiPart, REPO_FIELD_PASSWORD, password.toUtf8() );
        sdHttpMultiPartAppendField( multiPart, REPO_FIELD_INDEX, QByteArray::number(gindex) );

        mQueryType = SdRemoteQueryDownloadObject;
        QNetworkReply *reply = mNetworkManager->post( QNetworkRequest(QUrl( QStringLiteral("http://") + hostRepo + QStringLiteral("download.php"))), multiPart );
        multiPart->setParent(reply); // delete the multiPart with the reply
        }
      }
    }
  else {
    //Complete with download objects from remote repository
    //Begin to upload newest local objects to remote repository
    doUploadNextObject();
    }
  }





void SdObjectNetClient::doUploadNextObject()
  {
  QSettings s;
  QString author          = s.value( SDK_GLOBAL_AUTHOR ).toString();
  QString password        = s.value( SDK_GLOBAL_PASSWORD ).toString();
  int     localSyncIndex  = s.value( SDK_LOCAL_SYNC ).toInt();
  //int     remoteSyncIndex = s.value( SDK_REMOTE_SYNC ).toInt();
  QString hostRepo        = s.value( SDK_SERVER_REPO ).toString();
  if( !hostRepo.isEmpty() && !author.isEmpty() && !password.isEmpty() ) {
    QStringList list = sdLibraryStorage.getAfter( localSyncIndex, 1 );
    if( list.count() ) {
      QString uid = list.first();
      SdLibraryHeader header;
      if( sdLibraryStorage.header( uid, header ) ) {
        QByteArray object;
        QDataStream os( &object, QIODevice::WriteOnly );
        os << header << sdLibraryStorage.object(uid);

        //There object to upload to global repo
        //Prepare block for transmition
        QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

        sdHttpMultiPartAppendField( multiPart, REPO_FIELD_AUTHOR, author.toUtf8() );
        sdHttpMultiPartAppendField( multiPart, REPO_FIELD_PASSWORD, password.toUtf8() );
        sdHttpMultiPartAppendField( multiPart, REPO_FIELD_UID, uid.toUtf8() );
        sdHttpMultiPartAppendField( multiPart, REPO_FIELD_TIME, QByteArray::number(header.mTime) );
        sdHttpMultiPartAppendField( multiPart, REPO_FIELD_OBJECT, object.toHex() );

        qDebug() << "sync query upload" << uid;
        infoAppend( tr("Upload \"%d\"").arg(header.mName) );

        mQueryType = SdRemoteQueryUploadObject;
        QNetworkReply *reply = mNetworkManager->post( QNetworkRequest(QUrl( QStringLiteral("http://") + hostRepo + QStringLiteral("upload.php"))), multiPart );
        multiPart->setParent(reply); // delete the multiPart with the reply
        }
      else {
        //This object can't upload. Skeep it
        s.setValue( SDK_LOCAL_SYNC, ++localSyncIndex );
        doUploadNextObject();
        }
      }
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














