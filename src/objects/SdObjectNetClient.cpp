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
#include "SdContainerFile.h"
#include "library/SdLibraryStorage.h"

#include <QThread>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QHttpPart>
#include <QHttpMultiPart>
#include <QHostAddress>
#include <QSettings>
#include <QScopedPointer>
#include <QDebug>



#define REPO_FIELD_AUTHOR   QStringLiteral("author")
#define REPO_FIELD_PASSWORD QStringLiteral("password")
#define REPO_FIELD_EMAIL    QStringLiteral("email")
#define REPO_FIELD_RESULT   QStringLiteral("result")
#define REPO_FIELD_INDEX    QStringLiteral("index")
#define REPO_FIELD_UID      QStringLiteral("uid")
#define REPO_FIELD_TIME     QStringLiteral("time")
#define REPO_FIELD_OBJECT   QStringLiteral("object")


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
  mLockRemote(0)
  {

  mTimer.setInterval( 1000 );
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




void SdObjectNetClient::remoteUnlock()
  {
  if( mLockRemote )
    mLockRemote--;
  if( mLockRemote == 0 ) {
    mQueryType = SdRemoteQueryNone;  //Type of remote operation
    mObjectIndexList.clear();        //Object index list of newest objects from remote repository
    }
  }








//Begin registration process
void SdObjectNetClient::doRegister(const QString repo, const QString authorName, const QString password, const QString email)
  {
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

  emit registerStatus( -1, tr("Register start..."), email );
  mQueryType = SdRemoteQueryRegister;
  QNetworkReply *reply = mNetworkManager->post( QNetworkRequest(QUrl( QStringLiteral("http://") + fullRepo + QStringLiteral("register.php"))), multiPart );
  multiPart->setParent(reply); // delete the multiPart with the reply
  }












//By timer do syncronisation
void SdObjectNetClient::doSync()
  {
  mTimer.setInterval(10000);

  if( mLockRemote > 0 )
    return;

  if( mQueryType == SdRemoteQueryNone ) {
    QSettings s;
    int     remoteSyncIndex = s.value( SDK_REMOTE_SYNC ).toInt();
    QString hostRepo        = s.value( SDK_SERVER_REPO ).toString();
    if( !hostRepo.isEmpty() ) {
      emit remoteStatus( SdRemoteSync );

      //Prepare block for transmition
      QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

      sdHttpMultiPartAppendField( multiPart, REPO_FIELD_INDEX, QByteArray::number(remoteSyncIndex) );

      mQueryType = SdRemoteQueryList;
      QNetworkReply *reply = mNetworkManager->post( QNetworkRequest(QUrl( QStringLiteral("http://") + hostRepo + QStringLiteral("list.php"))), multiPart );
      multiPart->setParent(reply); // delete the multiPart with the reply
      }
    }
  }










//Receiv file from repository
void SdObjectNetClient::doFile(const QString fileName)
  {
  Q_UNUSED(fileName)
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
        emit registerStatus( 1, tr("Error when transfer: \"%1\"").arg(reply->errorString()), QString() );
        break;
      case SdRemoteQueryList :
      case SdRemoteQueryNone :
      case SdRemoteQueryUploadObject :
      case SdRemoteQueryDownloadObject :
        qDebug() << "finished Error when transfer " << reply->errorString();
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



//!
//! \brief cmRegister Reply received on register query
//! \param reply      Received reply
//!
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
        emit registerStatus( 0, tr("Registration successfully"), email );
        break;
      case 1 :
        emit registerStatus( 2, tr("Internal data. Check repo address"), email );
        break;
      case 2 :
        emit registerStatus( 3, tr("Failure data base connection. Try later"), email );
        break;
      case 3 :
        emit registerStatus( 4, tr("Can't insert this name into data base. Try another name"), email );
        break;
      case 4 :
        emit registerStatus( 5, tr("This name already present in data base with different password. Try another name or restore password."), email );
        break;
      default:
        emit registerStatus( 6, tr("Unresponsible code %1. Check repo address").arg(result), email );
        break;
      }
    }
  else {
    emit registerStatus( 7, tr("Unresponsible reply. Check repo address"), QString{} );
    }

  if( !success ) {
    QSettings s;
    s.remove( QStringLiteral(SDK_GLOBAL_AUTHOR) );
    s.remove( QStringLiteral(SDK_GLOBAL_PASSWORD) );
    }
  }
















//!
//! \brief cmSyncList Reply received on sync query. On next we download all objects from list which yet not downloaded
//! \param reply      Received reply
//!
void SdObjectNetClient::cmSyncList(const QJsonObject &reply)
  {
  if( mLockRemote > 0 )
    return;

  if( reply.value( QStringLiteral("result") ).toInt() == 0 ) {
    //Retrive object list from reply
    QJsonObject list = reply.value( QStringLiteral("list") ).toObject();
    mObjectIndexList.clear();
    if( list.count() ) {
      //qDebug() << "sync list received:" << list.count();
      infoAppend( tr("Sync list received %1 objects").arg(list.count()) );
      int indexMax = 0;
      for( auto it = list.constBegin(); it != list.constEnd(); it++ ) {
        QJsonObject obj = it.value().toObject();
        QString uid    = obj.value( REPO_FIELD_UID ).toString();
        int     time   = obj.value( REPO_FIELD_TIME ).toString().toInt();
        int     index  = it.key().toInt();
        if( index > indexMax ) indexMax = index;
        //qDebug() << uid << time;
        //Test if object newer than existing
        if( !SdLibraryStorage::instance()->cfIsOlderOrSame( uid, time ) ) {
          //Object is newer than existing or no object with this uid
          //so append object index to download list
          mObjectIndexList.append( index );
          //qDebug() << "[" << index << "]=" << uid;
          }
        }
      //Sort list by accedence
      if( mObjectIndexList.count() > 1 )
        std::sort( mObjectIndexList.begin(), mObjectIndexList.end() );
      if( mObjectIndexList.count() )
        indexMax = mObjectIndexList.first() - 1;
      QSettings s;
      s.setValue( SDK_REMOTE_SYNC, indexMax );
      }
    doDownloadNextObject();
    }
  }





//!
//! \brief cmDownloadObject Reply received on download object query. Downloaded object placed to local library storage
//! \param reply            Received reply
//!
void SdObjectNetClient::cmDownloadObject(const QJsonObject &reply)
  {
  if( mLockRemote > 0 )
    return;

  if( reply.value( QStringLiteral("result") ).toInt() == 0 ) {
    //Object download successfully
    int        remoteSyncIndex = reply.value( REPO_FIELD_INDEX ).toString().toInt();
    QByteArray objectHex       = reply.value( REPO_FIELD_OBJECT ).toString().toLatin1();

    QScopedPointer<SdContainerFile> item( sdObjectOnly<SdContainerFile>(SdObject::jsonCborCompressedFrom(QByteArray::fromHex( objectHex ))));

    if( !item.isNull() ) {
      infoAppend( tr("Downloaded \"%1\"").arg( item->getTitle() ) );

      SdLibraryStorage::instance()->cfObjectInsert( item.get() );
      //Exclude now downloaded object from upload
      SdLibraryStorage::instance()->cfObjectUploaded( item->getUid() );

      QSettings s;
      s.setValue( SDK_REMOTE_SYNC, remoteSyncIndex );
      mObjectIndexList.removeFirst();
      }
    }
  else if( reply.value( QStringLiteral("result") ).toInt() == 3 ) {
    //Запрошенный объект не найден - хрень какая-то
    mObjectIndexList.removeFirst();
    }
  doDownloadNextObject();
  }




//!
//! \brief cmUploadObject Reply received on upload object query. On next we continue upload all newest objects
//! \param reply          Received reply
//!
void SdObjectNetClient::cmUploadObject(const QJsonObject &reply)
  {
  if( mLockRemote > 0 )
    return;

  if( reply.value( QStringLiteral("result") ).toInt() == 0 ) {
    //Object uploaded successfully
    //qDebug() << "sync uploaded successfully";
    QString uid = reply.value(REPO_FIELD_UID).toString();
    SdLibraryStorage::instance()->cfObjectUploaded( uid );
    doUploadNextObject();
    }
  }














void SdObjectNetClient::cmFile(QDataStream &is)
  {
  Q_UNUSED(is)
//  emit remoteStatus( SdRemoteOn );

//  SdAuthorInfo info;
//  QString fileName;
//  QByteArray data;
//  is >> info >> fileName >> data;
  //  emit fileContents( info.result(), fileName, data );
  }





//!
//! \brief doDownloadNextObject Forms query to download next object from sync list
//!
void SdObjectNetClient::doDownloadNextObject()
  {
  if( mLockRemote > 0 )
    return;

  if( mObjectIndexList.count() ) {
    QSettings s;
    QString hostRepo        = s.value( SDK_SERVER_REPO ).toString();
    if( !hostRepo.isEmpty() ) {
      //Query next object
      //We need to download object
      //Prepare block for transmition
      QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

      sdHttpMultiPartAppendField( multiPart, REPO_FIELD_INDEX, QByteArray::number(mObjectIndexList.first()) );

      mQueryType = SdRemoteQueryDownloadObject;
      QNetworkReply *reply = mNetworkManager->post( QNetworkRequest(QUrl( QStringLiteral("http://") + hostRepo + QStringLiteral("download.php"))), multiPart );
      multiPart->setParent(reply); // delete the multiPart with the reply
      }
    }
  else {
    //Complete with download objects from remote repository
    //Begin to upload newest local objects to remote repository
    doUploadNextObject();
    }
  }





//!
//! \brief doUploadNextObject Form query to upload next newest object to global repository
//!
void SdObjectNetClient::doUploadNextObject()
  {
  if( mLockRemote > 0 )
    return;

  QSettings s;
  QString author          = s.value( SDK_GLOBAL_AUTHOR ).toString();
  QString password        = s.value( SDK_GLOBAL_PASSWORD ).toString();
  QString hostRepo        = s.value( SDK_SERVER_REPO ).toString();
  bool    uploadAvailable = s.value( SDK_UPLOAD_AUTO ).toBool();
  if( !hostRepo.isEmpty() && !author.isEmpty() && !password.isEmpty() && uploadAvailable ) {
    QScopedPointer<SdContainerFile> item( SdLibraryStorage::instance()->cfObjectUpload() );
    if( !item.isNull() ) {
      //There object to upload to global repo
      //Prepare block for transmition
      QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

      sdHttpMultiPartAppendField( multiPart, REPO_FIELD_AUTHOR, author.toUtf8() );
      sdHttpMultiPartAppendField( multiPart, REPO_FIELD_PASSWORD, password.toUtf8() );
      sdHttpMultiPartAppendField( multiPart, REPO_FIELD_UID, item->getUid().toUtf8() );
      sdHttpMultiPartAppendField( multiPart, REPO_FIELD_TIME, QByteArray::number( item->getTime() ) );
      sdHttpMultiPartAppendField( multiPart, REPO_FIELD_OBJECT, item->jsonCborCompressedTo().toHex() );

      //qDebug() << "sync query upload" << uid;
      infoAppend( tr("Upload \"%1\"").arg( item->getTitle() ) );

      mQueryType = SdRemoteQueryUploadObject;
      QNetworkReply *reply = mNetworkManager->post( QNetworkRequest(QUrl( QStringLiteral("http://") + hostRepo + QStringLiteral("upload.php"))), multiPart );
      multiPart->setParent(reply); // delete the multiPart with the reply
      return;
      }
    }
  }




//!
//! \brief infoAppend Append info to local info list and send signal with info to extern objects
//! \param info       Appended info
//!
void SdObjectNetClient::infoAppend(const QString info)
  {
  //Support count of info items in info list not more then 300 items
  if( mInfoList.count() >= 300 )
    mInfoList.removeFirst();

  mInfoList.append( info );

  //Send signal to extern objects
  emit informationAppended( info );
  }


