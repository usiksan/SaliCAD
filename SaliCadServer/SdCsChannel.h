/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/
#ifndef SDCSCHANNEL_H
#define SDCSCHANNEL_H

#include <QObject>
#include <QTcpSocket>
#include <QThread>

class SdCsChannel : public QObject
  {
    Q_OBJECT

    QTcpSocket *mSocket; //Socket, witch connection made with
    QThread    *mThread; //Thread, where SvChannel live in
  public:
    explicit SdCsChannel( QTcpSocket *socket, QObject *parent = nullptr);

  public slots:
    //On receiv bytes
    void onReceivBytes();

    //On write complete
    void onWriteComplete();

    //On connection complete
    void onConnectionClose();
  };

#endif // SDCSCHANNEL_H
