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

#include <QObject>
#include <QNetworkAccessManager>
#include <QTimer>
#include <QJsonArray>

enum SdRemoteStatus { SdRemoteOff, SdRemoteOn, SdRemoteSync };

enum SdRemoteQueryType {
  SdRemoteQueryNone,
  SdRemoteQueryRegister,
  SdRemoteQueryList,
  SdRemoteQueryUploadObject,
  SdRemoteQueryDownloadObject
  };

class SdObjectNetClient : public QObject
  {
    Q_OBJECT

    QNetworkAccessManager *mNetworkManager; //!< Network manager through witch we connect to global repository
    QTimer                 mTimer;          //!< Timer for periodic sync with global repository
    SdRemoteQueryType      mQueryType;      //!< Type of remote operation
    QJsonArray             mObjectList;     //!< Object list of newest objects from remote repository
    int                    mObjectIndex;    //!< Object index in mObjectList to receive object from remote repository
    QStringList            mInfoList;       //!< List for information items. When any event happens then information item appends
  public:
    explicit SdObjectNetClient(QObject *parent = nullptr);

    bool        isRegistered() const;

    QStringList infoList() const { return mInfoList; }

  signals:
    //Signal on process changed. Send current process state description and completion flag
    void process( QString desr, bool complete );

    //Signal send on complete object loading process
    // with result of loading and count of remain object to may be load
    void objectComplete( int result, int remain );

    //Signal send when on sync new object received
    void newObjectsReceived();

    //Signal on connection ok and fail
    void connectionStatus( QString msg, bool ok );

    //Signal on registration status
    void registerStatus( const QString msg, const QString email );

    //Signal on file receiv
    void fileContents( int result, QString fileName, QByteArray fileData );

    //Signal on remote status
    void remoteStatus( SdRemoteStatus st );

    //Append information item
    void informationAppended( const QString info );

  public slots:

    //!
    //! \brief doRegister Begin registration process or check registration status
    //!                   If this author with this password already registered or
    //!                   this author not registered then reply OK in other - duplicate
    //!                   author detected and must be enter another author name
    //! \param repo       Repository address, repo.salicad.com by default
    //! \param authorName Author name for registration
    //! \param password   Password for this author name
    //! \param email      E-mail for password restore
    //!
    void doRegister(const QString repo, const QString authorName, const QString password, const QString email );

    //Begin object receiving process
    void doObject( const QString hashId );

    //By timer do syncronisation
    void doSync();

    void startSync( bool start );

    //Receiv file from repository
    void doFile( const QString fileName );

    //!
    //! \brief finished Called when network reply finished
    //! \param reply    Reply witch being finished
    //!
    void finished( QNetworkReply *reply );

  private:
    void    cmRegister( const QJsonObject &reply );
    void    cmSyncList( const QJsonObject &reply );
    void    cmDownloadObject( const QJsonObject &reply );
    void    cmUploadObject( const QJsonObject &reply );
    void    cmFile( QDataStream &is );
    void    doDownloadNextObject();
    void    doUploadNextObject();

    //Append info to info list
    void    infoAppend( const QString info );
  };

//Main object for remote database communication
extern SdObjectNetClient *sdObjectNetClient;

#endif // SDOBJECTNETCLIENT_H
