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
#include "SaliCadServer/SdCsPacketInfo.h"
#include "library/SdLibraryStorage.h"

#include <QThread>
#include <QHostAddress>
#include <QSettings>


//Main object for remote database communication
SdObjectNetClient *sdObjectNetClient;


SdObjectNetClient::SdObjectNetClient(QObject *parent) :
  SdCsChannel( new QTcpSocket(), parent ),
  mCommand(0),
  mCommandSync(0)
  {
  QSettings s;
  mAuthor          = s.value( SDK_GLOBAL_AUTHOR ).toString();
  mKey             = s.value( SDK_MACHINE_KEY ).toString().toULongLong( nullptr, 32 );
  mLocalSyncIndex  = s.value( SDK_LOCAL_SYNC ).toInt();
  mRemoteSyncIndex = s.value( SDK_REMOTE_SYNC ).toInt();
  mHostIp          = s.value( SDK_SERVER_IP ).toString();

  mTimer.setInterval( 10000 );
  qDebug() << "registered" << isRegistered();
  if( isRegistered() )
    mTimer.start();

  connect( mSocket, &QTcpSocket::connected, this, &SdObjectNetClient::onConnected );
  connect( &mTimer, &QTimer::timeout, this, &SdObjectNetClient::doSync );
  connect( mSocket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error), [=] (QAbstractSocket::SocketError socketError) {
    emit process( tr("Connection error: %1").arg( socketError), true );
    });
  }






bool SdObjectNetClient::isRegistered() const
  {
  return !mHostIp.isEmpty() && !mAuthor.isEmpty() && mKey != 0;
  }








//Begin registration process
void SdObjectNetClient::doRegistration(const QString ip, const QString authorName, const QString email)
  {
  mTimer.stop();
  //Prepare block for transmition
  mHostIp         = ip;
  SdAuthorInfo info;
  info.mAuthor    = authorName;
  info.mEmail     = email;
  info.mRemain    = 100;
  info.mSyncIndex = 0;
  info.mKey       = 0;
  mBuffer.clear();
  QDataStream os( &mBuffer, QIODevice::WriteOnly );
  os << info;
  mCommand = SCPI_REGISTARTION_REQUEST;
  if( mSocket->state() != QAbstractSocket::ConnectedState ) {
    mSocket->connectToHost( QHostAddress(mHostIp), SD_DEFAULT_PORT );
    emit process( tr("Try connect to host %1").arg(mHostIp), false );
    }
  }





//Begin append machine
void SdObjectNetClient::doMachine(const QString ip, const QString authorName, quint64 key)
  {
  mTimer.stop();
  SdAuthorInfo info( authorName, key, 0 );
  //Prepare block for transmition
  mHostIp         = ip;
  info.mRemain    = 100;
  mBuffer.clear();
  QDataStream os( &mBuffer, QIODevice::WriteOnly );
  os << info;
  mCommand = SCPI_MACHINE_REQUEST;
  if( mSocket->state() != QAbstractSocket::ConnectedState ) {
    mSocket->connectToHost( QHostAddress(mHostIp), SD_DEFAULT_PORT );
    emit process( tr("Try connect to host %1").arg(mHostIp), false );
    }
  }





//Begin object receiving process
void SdObjectNetClient::doObject(const QString hashId)
  {
  mCommand = 0;
  mBuffer.clear();
  QDataStream os( &mBuffer, QIODevice::WriteOnly );
  SdAuthorInfo info( mAuthor, mKey, 0 );
  os << info << hashId;
  mCommand = SCPI_OBJECT_REQUEST;
  if( mSocket->state() != QAbstractSocket::ConnectedState ) {
    mSocket->connectToHost( QHostAddress(mHostIp), SD_DEFAULT_PORT );
    emit process( tr("Try connect to host %1").arg(mHostIp), false );
    }
  }









//When connected to host send to it prepared block
void SdObjectNetClient::onConnected()
  {
  qDebug() << "Connected to host" << mHostIp;
  emit process( tr("Connected to host %1").arg(mHostIp), false );
  startTransmit();
  }









//By timer do syncronisation
void SdObjectNetClient::doSync()
  {
  mTimer.setInterval( 180000 );
  if( !mHostIp.isEmpty() ) {
    qDebug() << "doSync";
    mCommandSync = 0;
    mBufferSync.clear();
    QDataStream os( &mBufferSync, QIODevice::WriteOnly );
    SdAuthorInfo info( mAuthor, mKey, mRemoteSyncIndex );
    os << info;

    //Scan object list for last entered
    QStringList list = sdLibraryStorage.getAfter( mLocalSyncIndex );
    //For each object write header and object itself
    for( const QString &hash : list ) {
      SdLibraryHeader hdr;
      if( sdLibraryStorage.header( hash, hdr ) ) {
        //Header readed successfull, transmit header and object
        os << hdr << sdLibraryStorage.object(hash);
        }
      }
    qDebug() << "sync objects " << list.count();
    mLocalSyncCount = list.count();
    mCommandSync = SCPI_SYNC_REQUEST;
    if( mSocket->state() != QAbstractSocket::ConnectedState ) {
      mSocket->connectToHost( QHostAddress(mHostIp), SD_DEFAULT_PORT );
      emit process( tr("Try connect to host %1").arg(mHostIp), false );
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










void SdObjectNetClient::onBlockReceived(int cmd, QDataStream &is)
  {
  SdCadServerVersion version;
  is >> version;
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
  SdAuthorInfo info;
  is >> info;
  qDebug() << "cmRegistrationInfo" << info.mKey;
  if( info.isSuccessfull() ) {
    QSettings s;
    s.setValue( SDK_GLOBAL_AUTHOR, info.mAuthor );
    s.setValue( SDK_MACHINE_KEY, QString::number(info.mKey, 32) );
    s.setValue( SDK_SERVER_IP, mHostIp );
    s.setValue( SDK_REMOTE_REMAIN, QString::number(info.mRemain) );
    mAuthor = info.mAuthor;
    mKey    = info.mKey;
    //Reset syncronisation process
    mLocalSyncIndex = mLocalSyncCount = 0;
    s.setValue( SDK_REMOTE_SYNC, mRemoteSyncIndex );
    s.setValue( SDK_LOCAL_SYNC, mLocalSyncIndex );
    //doSync();
    emit process( tr("Registration successfull"), false );
    }
  else emit process( tr("Failure registration. ") + error(info.result()), false );
  emit registrationComplete( info.mAuthor, info.mEmail, info.mKey, info.mRemain, info.result() );
  }




void SdObjectNetClient::cmSyncList(QDataStream &is)
  {
  SdAuthorInfo info;
  is >> info;
  qDebug() << "cmSyncList" << info.mAuthor << info.mKey << info.mRemain;
  if( info.isSuccessfull() ) {
    mLocalSyncIndex += mLocalSyncCount;

    //Here is headers list (may be empty)
    int updateCount = 0;
    while( !is.atEnd() ) {
      SdLibraryHeader hdr;
      is >> hdr;
      sdLibraryStorage.setHeader( hdr );
      updateCount++;
      }
    qDebug() << "synced" << updateCount << mLocalSyncCount;
    if( updateCount != 0 || mLocalSyncCount != 0 ) {
      mRemoteSyncIndex += updateCount;
      QSettings s;
      s.setValue( SDK_REMOTE_SYNC, mRemoteSyncIndex );
      s.setValue( SDK_LOCAL_SYNC, mLocalSyncIndex );
      emit newObjectsReceived();
      doSync();
      }
    sdLibraryStorage.flush();
    }
  }






void SdObjectNetClient::cmObject(QDataStream &is)
  {
  SdAuthorInfo info;
  is >> info;
  qDebug() << "cmObject" << info.mAuthor << info.mKey << info.mRemain;
  if( info.isSuccessfull() ) {
    //For object header
    SdLibraryHeader header;
    //For object store
    QByteArray obj;

    is >> header >> obj;

    sdLibraryStorage.insert( header, obj );

    QSettings s;
    s.setValue( SDK_REMOTE_REMAIN, QString::number(info.mRemain) );
    }
  else emit process( error(info.result()), false );
  emit objectComplete( info.result(), info.mRemain );
  }



void SdObjectNetClient::startTransmit()
  {
  if( mCommandSync ) {
    emit process( tr("Syncronisation request"), false );
    writeBlock( mCommandSync, mBufferSync );
    mCommandSync = 0;
    }
  else if( mCommand ) {
    QString msg;
    switch( mCommand ) {
      case SCPI_REGISTARTION_REQUEST : msg = tr("Registration request"); break;
      case SCPI_MACHINE_REQUEST : msg = tr("Machine appendion request"); break;
      case SCPI_OBJECT_REQUEST : msg = tr("Object request"); break;
      }
    emit process( msg, false );
    writeBlock( mCommand, mBuffer );
    mCommand = 0;
    }
  else {
    qDebug() << "Disconnect";
    mSocket->disconnectFromHost();
    }
  }




QString SdObjectNetClient::error(int code)
  {
  switch( code ) {
    case SCPE_AUTHOR_ALREADY_PRESENT : return tr("Author with this name already present. Select another name.");
    case SCPE_AUTHOR_IS_EMPTY : return tr("Author can't be empty string.");
    case SCPE_MACHINE_LIMIT : return tr("No more machines available. Limit reached.");
    case SCPE_NOT_REGISTERED : return tr("Author with this name and key is not registered.");
    case SCPE_OBJECT_LIMIT : return tr("No more object load available. Limit reached.");
    case SCPE_OBJECT_NOT_FOUND : return tr("Object not found.");
    }
  return tr("Undefined code %1").arg(code);
  }






