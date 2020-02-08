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
//  mAuthor          = s.value( SDK_GLOBAL_AUTHOR ).toString();
//  mKey             = s.value( SDK_MACHINE_KEY ).toString().toULongLong( nullptr, 32 );
//  mLocalSyncIndex  = s.value( SDK_LOCAL_SYNC ).toInt();
//  mRemoteSyncIndex = s.value( SDK_REMOTE_SYNC ).toInt();
//  mHostIp          = s.value( SDK_SERVER_IP ).toString();

  mTimer.setInterval( 10000 );
  qDebug() << "registered" << isRegistered();
  if( isRegistered() )
    mTimer.start();

  connect( mSocket, &QTcpSocket::connected, this, &SdObjectNetClient::onConnected );
  connect( &mTimer, &QTimer::timeout, this, &SdObjectNetClient::doSync );
  connect( mSocket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error), [=] (QAbstractSocket::SocketError socketError) {
    auto msg = tr("Connection error: %1").arg( socketError);
    emit process( msg, true );
    emit connectionStatus( msg, false );
    });
  }






bool SdObjectNetClient::isRegistered() const
  {
  QSettings s;
  QString  author          = s.value( SDK_GLOBAL_AUTHOR ).toString();
  quint64  key             = s.value( SDK_MACHINE_KEY ).toString().toULongLong( nullptr, 32 );
  QString  hostIp          = s.value( SDK_SERVER_IP ).toString();
  return !hostIp.isEmpty() && !author.isEmpty() && key != 0;
  }








//Begin registration process
void SdObjectNetClient::doRegistration(const QString ip, const QString authorName, const QString email)
  {
  mTimer.stop();
  //Prepare block for transmition
  QSettings s;
  s.setValue( SDK_SERVER_IP, ip );
  //mHostIp         = ip;
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
    mSocket->connectToHost( QHostAddress(ip), SD_DEFAULT_PORT );
    emit process( tr("Try connect to host %1").arg(ip), false );
    }
  }





//Begin append machine
void SdObjectNetClient::doMachine(const QString ip, const QString authorName, quint64 key)
  {
  mTimer.stop();
  SdAuthorInfo info( authorName, key, 0 );
  //Prepare block for transmition
  QSettings s;
  s.setValue( SDK_SERVER_IP, ip );
  //mHostIp         = ip;
  info.mRemain    = 100;
  mBuffer.clear();
  QDataStream os( &mBuffer, QIODevice::WriteOnly );
  os << info;
  mCommand = SCPI_MACHINE_REQUEST;
  if( mSocket->state() != QAbstractSocket::ConnectedState ) {
    mSocket->connectToHost( QHostAddress(ip), SD_DEFAULT_PORT );
    emit process( tr("Try connect to host %1").arg(ip), false );
    }
  }





//Begin object receiving process
void SdObjectNetClient::doObject(const QString hashId)
  {
  emit remoteStatus( SdRemoteSync );
  mCommand = 0;
  mBuffer.clear();
  QDataStream os( &mBuffer, QIODevice::WriteOnly );
  QSettings s;
  QString author          = s.value( SDK_GLOBAL_AUTHOR ).toString();
  quint64 key             = s.value( SDK_MACHINE_KEY ).toString().toULongLong( nullptr, 32 );
  QString hostIp          = s.value( SDK_SERVER_IP ).toString();

  SdAuthorInfo info( author, key, 0 );
  os << info << hashId;
  mCommand = SCPI_OBJECT_REQUEST;
  if( mSocket->state() != QAbstractSocket::ConnectedState ) {
    mSocket->connectToHost( QHostAddress(hostIp), SD_DEFAULT_PORT );
    emit process( tr("Try connect to host %1").arg(hostIp), false );
    }
  }









//When connected to host send to it prepared block
void SdObjectNetClient::onConnected()
  {
  //qDebug() << "Connected to host" << mHostIp;
  QSettings s;
  QString hostIp = s.value( SDK_SERVER_IP ).toString();
  auto msg = tr("Connected to host %1").arg(hostIp);
  emit process( msg, false );
  emit connectionStatus( msg, true );
  infoAppend( msg );
  startTransmit();
  }









//By timer do syncronisation
void SdObjectNetClient::doSync()
  {
  mTimer.setInterval( 180000 );
  QSettings s;
  QString author          = s.value( SDK_GLOBAL_AUTHOR ).toString();
  quint64 key             = s.value( SDK_MACHINE_KEY ).toString().toULongLong( nullptr, 32 );
  int     localSyncIndex  = s.value( SDK_LOCAL_SYNC ).toInt();
  int     remoteSyncIndex = s.value( SDK_REMOTE_SYNC ).toInt();
  QString hostIp          = s.value( SDK_SERVER_IP ).toString();
  if( !hostIp.isEmpty() ) {
    remoteStatus( SdRemoteSync );
    //qDebug() << "doSync";
    mCommandSync = 0;
    mBufferSync.clear();
    QDataStream os( &mBufferSync, QIODevice::WriteOnly );
    SdAuthorInfo info( author, key, remoteSyncIndex );
    os << info;

    //Scan object list for last entered
    QStringList list = sdLibraryStorage.getAfter( localSyncIndex );
    //For each object write header and object itself
    for( const QString &hash : list ) {
      SdLibraryHeader hdr;
      if( sdLibraryStorage.header( hash, hdr ) ) {
        //Header readed successfull, transmit header and object
        os << hdr << sdLibraryStorage.object(hash);
        }
      }
    qDebug() << "sync objects transmited" << list.count();
    mLocalSyncCount = list.count();
    mCommandSync = SCPI_SYNC_REQUEST;
    if( mSocket->state() != QAbstractSocket::ConnectedState ) {
      mSocket->connectToHost( QHostAddress(hostIp), SD_DEFAULT_PORT );
      emit process( tr("Try connect to host %1").arg(hostIp), false );
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





//Check registration
void SdObjectNetClient::doCheck()
  {
  QSettings s;
  QString author          = s.value( SDK_GLOBAL_AUTHOR ).toString();
  quint64 key             = s.value( SDK_MACHINE_KEY ).toString().toULongLong( nullptr, 32 );
  //  mLocalSyncIndex  = s.value( SDK_LOCAL_SYNC ).toInt();
  //  mRemoteSyncIndex = s.value( SDK_REMOTE_SYNC ).toInt();
  QString hostIp          = s.value( SDK_SERVER_IP ).toString();

  mBuffer.clear();
  QDataStream os( &mBuffer, QIODevice::WriteOnly );
  SdAuthorInfo info( author, key, 0 );
  os << info;
  mCommand = SCPI_ACCESS_CHECK_REQUEST;
  if( mSocket->state() != QAbstractSocket::ConnectedState ) {
    mSocket->connectToHost( QHostAddress(hostIp), SD_DEFAULT_PORT );
    QString msg = tr("Try connect to host %1").arg(hostIp);
    emit process( msg, false );
    infoAppend( msg );
    }
  }





//Receiv file from repository
void SdObjectNetClient::doFile(const QString fileName)
  {
  emit remoteStatus( SdRemoteSync );
  QSettings s;
  QString author          = s.value( SDK_GLOBAL_AUTHOR ).toString();
  quint64 key             = s.value( SDK_MACHINE_KEY ).toString().toULongLong( nullptr, 32 );
  //  mLocalSyncIndex  = s.value( SDK_LOCAL_SYNC ).toInt();
  //  mRemoteSyncIndex = s.value( SDK_REMOTE_SYNC ).toInt();
  QString hostIp          = s.value( SDK_SERVER_IP ).toString();

  mBuffer.clear();
  QDataStream os( &mBuffer, QIODevice::WriteOnly );
  SdAuthorInfo info( author, key, 0 );
  os << info << fileName;
  mCommand = SCPI_FILE_REQUEST;
  if( mSocket->state() != QAbstractSocket::ConnectedState ) {
    mSocket->connectToHost( QHostAddress(hostIp), SD_DEFAULT_PORT );
    emit process( tr("Try connect to host %1").arg(hostIp), false );
    }
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

    case SCPI_FILE :
      cmFile( is );
      break;

    case SCPI_ACCESS_CHECK_ACK :
      cmCheck( is );
    }
  startTransmit();
  }




void SdObjectNetClient::cmRegistrationInfo(QDataStream &is)
  {
  SdAuthorInfo info;
  is >> info;
  //qDebug() << "cmRegistrationInfo" << info.mKey;
  if( info.isSuccessfull() ) {
    QSettings s;
    s.setValue( SDK_GLOBAL_AUTHOR, info.mAuthor );
    s.setValue( SDK_MACHINE_KEY, QString::number(info.mKey, 32) );
    s.setValue( SDK_REMOTE_REMAIN, QString::number(info.mRemain) );
    //Reset syncronisation process
    mLocalSyncCount = 0;
    s.setValue( SDK_REMOTE_SYNC, 0 );
    s.setValue( SDK_LOCAL_SYNC, 0 );
    //doSync();
    auto msg = tr("Registration successfull");
    emit process( msg, false );
    emit registrationStatus( msg, true );
    }
  else {
    auto msg = tr("Failure registration. ") + error(info.result());
    emit process( msg, false );
    emit registrationStatus( msg, false );
    }
  emit registrationComplete( info.mAuthor, info.mEmail, info.mKey, info.mRemain, info.result() );
  }




void SdObjectNetClient::cmSyncList(QDataStream &is)
  {
  QSettings s;
  int     localSyncIndex  = s.value( SDK_LOCAL_SYNC ).toInt();
  int     remoteSyncIndex = s.value( SDK_REMOTE_SYNC ).toInt();

  SdAuthorInfo info;
  is >> info;
  //qDebug() << "cmSyncList" << info.mAuthor << info.mKey << info.mRemain;
  if( info.isSuccessfull() ) {
    emit remoteStatus( SdRemoteOn );

    localSyncIndex += mLocalSyncCount;

    //Here is headers list (may be empty)
    int updateCount = 0;
    while( !is.atEnd() ) {
      SdLibraryHeader hdr;
      is >> hdr;
      sdLibraryStorage.setHeader( hdr );
      updateCount++;
      }
    if( updateCount != 0 || mLocalSyncCount != 0 ) {
      qDebug() << "synced received" << updateCount << "transfered" << mLocalSyncCount;
      remoteSyncIndex += updateCount;

      s.setValue( SDK_REMOTE_SYNC, remoteSyncIndex );
      s.setValue( SDK_LOCAL_SYNC, localSyncIndex );
      emit newObjectsReceived();
      doSync();
      }
    sdLibraryStorage.flush();
    }
  else
    emit remoteStatus( SdRemoteOff );
  }






void SdObjectNetClient::cmObject(QDataStream &is)
  {
  SdAuthorInfo info;
  is >> info;
  qDebug() << "cmObject" << info.mAuthor << info.mKey << info.mRemain;
  if( info.isSuccessfull() ) {
    emit remoteStatus( SdRemoteOn );

    //For object header
    SdLibraryHeader header;
    //For object store
    QByteArray obj;

    is >> header >> obj;

    sdLibraryStorage.insert( header, obj );

    QSettings s;
    s.setValue( SDK_REMOTE_REMAIN, QString::number(info.mRemain) );
    }
  else {
    emit process( error(info.result()), false );
    emit remoteStatus( SdRemoteOff );
    }
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



void SdObjectNetClient::cmCheck(QDataStream &is)
  {
  SdAuthorInfo info;
  is >> info;
  if( info.isSuccessfull() ) {
    auto msg = tr("%1 registered").arg( info.mAuthor );
    emit process( msg, false );
    emit registrationStatus( msg, true );
    }
  else {
    auto msg = tr("%1 not registered or key failure").arg(info.mAuthor);
    emit process( msg, false );
    emit registrationStatus( msg, false );
    }
  }




void SdObjectNetClient::cmFile(QDataStream &is)
  {
  emit remoteStatus( SdRemoteOn );

  SdAuthorInfo info;
  QString fileName;
  QByteArray data;
  is >> info >> fileName >> data;
  emit fileContents( info.result(), fileName, data );
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






