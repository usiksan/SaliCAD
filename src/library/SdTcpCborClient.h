#ifndef SDTCPCBORCLIENT_H
#define SDTCPCBORCLIENT_H

#include <QObject>
#include <QByteArray>
#include <QString>

class QTcpSocket;
class QCborValue;

class SdTcpCborClient : public QObject
  {
    Q_OBJECT

    QTcpSocket* mSocket;    //!< Socket for connection
    bool        mConnected; //!< Connection indicator
  public:
    SdTcpCborClient(QObject* parent = nullptr);
    ~SdTcpCborClient();

    // Основные методы
    void       openSocket(const QString& host, quint16 port);
    QCborValue transferSocket(const QCborValue& data);
    void       closeSocket();

    bool       isConnected() const;

  };

#endif // SDTCPCBORCLIENT_H
