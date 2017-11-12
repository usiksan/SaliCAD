/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Client for dataBase server communicate
*/
#ifndef SDOBJECTNETCLIENT_H
#define SDOBJECTNETCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QTimer>


class SdObjectNetClient : public QObject
  {
    Q_OBJECT

    QString     mAuthor;
    QString     mKey;
    QTcpSocket *mSocket;
    QTimer     *mTimer;
  public:
    explicit SdObjectNetClient(QObject *parent = nullptr);

  signals:
    void objectComplete( const QString hashId, bool successfull );

    void loginComplete( const QString author, const QString key, int limit, int delivered );
  public slots:
    //Begin object receiving process
    void needObject( const QString hashId );

    //Begin registration process
    void needRegistration(const QString ip, const QString name, const QString description );

    //Begin login process
    void needLogin( const QString name, const QString key );

    //Begin background process
    void start();
  };

//Main object for remote database communication
extern SdObjectNetClient *sdObjectNetClient;

#endif // SDOBJECTNETCLIENT_H
