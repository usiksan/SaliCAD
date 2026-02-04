#ifndef SDTCPCBORCLIENT_H
#define SDTCPCBORCLIENT_H

#include <QObject>
#include <QByteArray>
#include <QString>

class QTcpSocket;
class QCborValue;
class QCborMap;

class SdTcpCborClient : public QObject
  {
    Q_OBJECT

    QTcpSocket* mSocket;    //!< TCP socket instance for network communication
    bool        mConnected; //!< Connection state indicator (true if connected)
  public:
    //!
    //! \brief Constructor for SdTcpCborClient
    //! \param parent Parent QObject for Qt object hierarchy (optional)
    //!
    SdTcpCborClient(QObject* parent = nullptr);


    //!
    //! \brief Destructor - ensures proper cleanup of socket connection
    //!
    ~SdTcpCborClient();


    //!
    //! \brief openSocket Establishes TCP connection to the specified host and port
    //!
    //! Attempts to connect to the given host using TCP. The host parameter can be
    //! either an IP address (IPv4 or IPv6) or a hostname. If a hostname is provided,
    //! it will be resolved to an IP address using DNS. The connection is synchronous
    //! with a timeout.
    //!
    //! \param host Target host as IP address (e.g., "192.168.1.1") or hostname (e.g., "example.com")
    //! \param port Target port number (1-65535)
    //! \throws std::runtime_error if connection fails or host cannot be resolved
    //!
    void       openSocket(const QString& host, quint16 port);


    //!
    //! \brief transferSocket Sends CBOR data and waits for response
    //!
    //! Serializes the provided CBOR value, prepends a 4-byte big-endian length prefix,
    //! sends it over the established connection, and waits for a response. The response
    //! is expected to have the same length-prefixed format. This method is synchronous
    //! and blocks until the complete response is received or a timeout occurs.
    //!
    //! \param data CBOR value to send to the server
    //! \return     Deserialized CBOR response from the server
    //! \throws     std::runtime_error if socket is not connected, transmission fails,
    //!             or response cannot be properly received/deserialized
    //!
    QCborValue transferSocket(const QCborValue& data);

    QCborMap   transferMap( const QCborMap &src );


    //!
    //! \brief closeSocket Closes the TCP connection gracefully
    //!
    //! Disconnects from the remote host and closes the socket. If no connection
    //! is active, this method does nothing.
    //!
    void       closeSocket();

    //!
    //! \brief Checks current connection state
    //! \return true if socket is connected to a remote host, false otherwise
    //!
    bool       isConnected() const;

  };

#endif // SDTCPCBORCLIENT_H
