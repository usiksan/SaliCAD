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

class SdObjectNetClient : public QObject
  {
    Q_OBJECT
  public:
    explicit SdObjectNetClient(QObject *parent = nullptr);

  signals:

  public slots:
  };

//Main object for remote database communication
extern SdObjectNetClient *sdObjectNetClient;

#endif // SDOBJECTNETCLIENT_H
