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

class SdObjectNetClient : public SdCsChannel
  {
    Q_OBJECT

    SdAuthorInfo  mAuthorInfo;
    SdAuthorInfo  mAuthorResult;
    QString       mHostIp;
    QTimer        mTimer;
    QByteArray    mBuffer;
    int           mCommand;
    QByteArray    mBufferSync;
    int           mCommandSync;
  public:
    explicit SdObjectNetClient(QObject *parent = nullptr);

    bool isRegistered() const;

  signals:
    void process( QString desr, bool complete );

    void objectComplete( int result );

    void registrationComplete( const QString authorName, const QString email, const QString key, int remain );
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

    // SdCsChannel interface
  public:
    virtual void onBlockReceived( int cmd, QDataStream &is ) override;

  private:
    void cmRegistrationInfo( QDataStream &is );
    void cmSyncList( QDataStream &is );
    void cmObject( QDataStream &is );
    void startTransmit();
  };

//Main object for remote database communication
extern SdObjectNetClient *sdObjectNetClient;

#endif // SDOBJECTNETCLIENT_H
