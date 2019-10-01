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
#ifndef SDOBJECTNETCLIENT_H
#define SDOBJECTNETCLIENT_H

#include "SaliCadServer/SdCsChannel.h"
#include "SaliCadServer/SdCsPacketInfo.h"
#include <QObject>
#include <QTcpSocket>
#include <QTimer>

enum SdRemoteStatus { SdRemoteOff, SdRemoteOn, SdRemoteSync };

class SdObjectNetClient : public SdCsChannel
  {
    Q_OBJECT

//    QString       mAuthor;          //Author name
//    QString       mEmail;
//    quint64       mKey;             //Author key
//    qint32        mRemain;          //Remain object count for loading
//    qint32        mLocalSyncIndex;  //Index of last sync in local storage
//    qint32        mRemoteSyncIndex; //Index of last sync in remote storage
//    QString       mHostIp;
    QTimer        mTimer;
    QByteArray    mBuffer;
    QByteArray    mBufferSync;
    int           mCommand;
    int           mCommandSync;
    int           mLocalSyncCount;
    quint8        padding[4];
  public:
    explicit SdObjectNetClient(QObject *parent = nullptr);

    bool isRegistered() const;

  signals:
    //Signal on process changed. Send current process state description and completion flag
    void process( QString desr, bool complete );

    //Signal send on complete object loading process
    // with result of loading and count of remain object to may be load
    void objectComplete( int result, int remain );

    //Signal send on user registration complete
    // with accnowledges all query params
    void registrationComplete( const QString authorName, const QString email, quint64 key, int remain, int result );

    //Signal send when on sync new object received
    void newObjectsReceived();

    //Signal on connection ok and fail
    void connectionStatus( QString msg, bool ok );

    //Signal on registration status
    void registrationStatus( QString msg, bool ok );

    //Signal on file receiv
    void fileContents( int result, QString fileName, QByteArray fileData );

    //Signal on remote status
    void remoteStatus( SdRemoteStatus st );

  public slots:

    //Begin registration process
    void doRegistration(const QString ip, const QString authorName, const QString email );

    //Begin append machine
    void doMachine(const QString ip, const QString authorName, quint64 key );

    //Begin object receiving process
    void doObject( const QString hashId );

    //When connected to host send to it prepared block
    void onConnected();

    //By timer do syncronisation
    void doSync();

    void startSync( bool start );

    //Check registration
    void doCheck();

    //Receiv file from repository
    void doFile( const QString fileName );

    // SdCsChannel interface
  public:
    virtual void onBlockReceived( int cmd, QDataStream &is ) override;

    //Conver error code to string description
    static QString error( int code );
  private:
    void    cmRegistrationInfo( QDataStream &is );
    void    cmSyncList( QDataStream &is );
    void    cmObject( QDataStream &is );
    void    startTransmit();
    void    cmCheck( QDataStream &is );
    void    cmFile( QDataStream &is );
  };

//Main object for remote database communication
extern SdObjectNetClient *sdObjectNetClient;

#endif // SDOBJECTNETCLIENT_H
