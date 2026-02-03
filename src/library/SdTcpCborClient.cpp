#include "SdTcpCborClient.h"
#include <QCborStreamWriter>
#include <QCborStreamReader>
#include <QDataStream>
#include <QtEndian>
#include <QHostAddress>
#include <QHostInfo>
#include <QTcpSocket>
#include <QCborValue>

SdTcpCborClient::SdTcpCborClient(QObject* parent)
  : QObject(parent)
  , mSocket(nullptr)
  , mConnected(false)
  {
  mSocket = new QTcpSocket(this);
  }




SdTcpCborClient::~SdTcpCborClient()
  {
  closeSocket();
  }




void SdTcpCborClient::openSocket(const QString& host, quint16 port)
  {
  if( !mConnected ) {
    qDebug() << "Connecting to " << host << " : " << port;

    // Преобразуем строку в QHostAddress
    QHostAddress hostAddress;

    // Пробуем распарсить как IPv4 или IPv6
    if( !hostAddress.setAddress(host) ) {
      // Если не IP-адрес, пытаемся разрешить доменное имя
      qDebug() << "Resolving hostname: " << host;

      QHostInfo hostInfo = QHostInfo::fromName(host);
      if( hostInfo.error() != QHostInfo::NoError ) {
        QString error = QString("Failed to resolve hostname '%1': %2").arg(host, hostInfo.errorString());
        qDebug() << error;
        throw std::runtime_error(error.toStdString());
        }

      // Берем первый IP-адрес из списка
      if( hostInfo.addresses().isEmpty() ) {
        QString error = QString("No IP addresses found for hostname '%1'").arg(host);
        qDebug() << error;
        throw std::runtime_error(error.toStdString());
        }

      hostAddress = hostInfo.addresses().first();
      qDebug() << QString("Resolved to IP: %1").arg(hostAddress.toString());
      }

    // Подключаемся
    mSocket->connectToHost(hostAddress, port);

    // Ждем подключения с таймаутом
    if( !mSocket->waitForConnected(5000) ) {
      QString error = QString("Connection failed: %1").arg(mSocket->errorString());
      qDebug() << error;
      throw std::runtime_error(error.toStdString());
      }

    mConnected = true;
    }
  }





void SdTcpCborClient::closeSocket()
  {
  if (mSocket->state() == QAbstractSocket::ConnectedState) {
    mSocket->disconnectFromHost();
    if (mSocket->state() != QAbstractSocket::UnconnectedState) {
      mSocket->waitForDisconnected(1000);
      }
    }
  mConnected = false;
  }






bool SdTcpCborClient::isConnected() const
  {
  return mConnected;
  }





QCborValue SdTcpCborClient::transferSocket(const QCborValue& data)
  {
  if( !mConnected ) {
    qDebug() << "Socket is not connected";
    throw std::runtime_error("Socket is not connected");
    }

  qDebug() << "Preparing data for transmission...";

  // 1. Сериализуем данные с префиксом длины
  // Сериализуем CBOR
  QByteArray cborData = data.toCbor();

  // Создаем массив с префиксом длины
  QByteArray dataToSend( 4, 0 );

  // Записываем длину (4 байта, big-endian)
  quint32 size = cborData.size();
  cborData[0] = (size >> 24) & 0xff;
  cborData[1] = (size >> 16) & 0xff;
  cborData[2] = (size >> 8) & 0xff;
  cborData[3] = (size >> 0) & 0xff;

  // Копируем CBOR данные
  dataToSend.append( cborData );

  // 2. Отправляем данные
  qDebug() << "Sending bytes " << dataToSend.size();

  qint64 written = mSocket->write(dataToSend);
  if( written != dataToSend.size() ) {
    QString error = QString("Failed to write all data. Written: %1 of %2").arg(written).arg(dataToSend.size());
    qDebug() << error;
    throw std::runtime_error(error.toStdString());
    }

  // Ждем отправки
  if( !mSocket->waitForBytesWritten(5000) ) {
    qDebug() << "Timeout while waiting for data to be sent";
    throw std::runtime_error("Timeout while waiting for data to be sent");
    }

  qDebug() << "Data sent, waiting for response...";

  // 3. Получаем ответ
  // 1. Сначала читаем 4 байта длины
  const int headerSize = 4;

  // Ждем префикс длины
  if( !mSocket->waitForReadyRead(10000) ) {
    throw std::runtime_error("Timeout waiting for response header");
    }

  // Читаем пока не получим все 4 байта
  while( mSocket->bytesAvailable() < headerSize ) {
    if( !mSocket->waitForReadyRead(1000) ) {
      qDebug() << "Timeout waiting for complete header";
      throw std::runtime_error("Timeout waiting for complete header");
      }
    }

  QByteArray lengthData = mSocket->read(headerSize);
  if( lengthData.size() != headerSize ) {
    qDebug() << "Failed to read length prefix";
    throw std::runtime_error("Failed to read length prefix");
    }

  // 2. Получаем длину сообщения
  quint32 messageLength;
  messageLength = (lengthData[0] & 0xff) << 24;
  messageLength |= (lengthData[1] & 0xff) << 16;
  messageLength |= (lengthData[2] & 0xff) << 8;
  messageLength |= (lengthData[3] & 0xff);

  if( messageLength == 0 ) {
    qDebug() << "Empty message received";
    throw std::runtime_error("Empty message received");
    }

  if( messageLength > 10 * 1024 * 1024 ) { // 10 MB лимит
    qDebug() << "Message too large";
    throw std::runtime_error("Message too large");
    }

  qDebug() << "Expecting bytes of CBOR data " << messageLength;

  // 3. Читаем само сообщение
  QByteArray messageData;
  messageData.reserve(messageLength);

  while( messageData.size() < static_cast<int>(messageLength) ) {
    if( !mSocket->waitForReadyRead(10000) ) {
      qDebug() << "Timeout waiting for message data";
      throw std::runtime_error("Timeout waiting for message data");
      }

    // Читаем доступные данные
    qint64 bytesToRead = qMin( static_cast<qint64>(messageLength - messageData.size()), mSocket->bytesAvailable() );

    if( bytesToRead > 0 ) {
      messageData.append(mSocket->read(bytesToRead));
      }

    qDebug() << "Received " << messageData.size() << " of bytes" << messageLength;
    }

  qDebug() << "Received bytes, deserializing..." << messageData.size();

  // 4. Десериализуем ответ
  QCborStreamReader reader(messageData);
  QCborValue result = QCborValue::fromCbor(reader);

  if( reader.lastError() != QCborError::NoError ) {
    qDebug() << "Failed to deserialize CBOR response";
    throw std::runtime_error("Failed to deserialize CBOR response");
    }

  return result;
  }







