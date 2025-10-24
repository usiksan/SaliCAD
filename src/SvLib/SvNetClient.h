/*
  Project  "SvLib - common library"
  Author
    Alexander Sibilev
  Internet
    www.saliLab.com

  Description
    SvNetClient - base class for network communication
*/
#ifndef SVNETCLIENT_H
#define SVNETCLIENT_H

#include <QObject>
#include <QHttpMultiPart>
#include <QNetworkAccessManager>
#include <QTimer>
#include <QJsonObject>

//!
//! \brief The SvNetQuery class for simlify query building
//!
//!
class SvNetQuery
  {
    QHttpMultiPart *mMultiPart; //!< Multipart http query. In creates on demand
  public:
    SvNetQuery() : mMultiPart( nullptr ) {}

    //!
    //! \brief multiPartGet Return multipart http query and begins new query building loop
    //! \return             Multipart http query
    //!
    QHttpMultiPart *multiPartGet() { auto tmp(mMultiPart); mMultiPart = nullptr; return tmp; }

    //!
    //! \brief appendField Append one field with contens to query
    //! \param fieldName   Field name
    //! \param data        Data for field
    //! \return            Query for subsequential call
    //!
    SvNetQuery &appendField( const QString fieldName, const QByteArray data );

    //!
    //! \brief appendFieldInt Append one field with int to query
    //! \param fieldName      Field name
    //! \param value          Int value. It converted to string representation
    //! \return               Query for subsequential call
    //!
    SvNetQuery &appendFieldInt( const QString fieldName, int value ) { return appendField( fieldName, QByteArray::number(value) ); }

    //!
    //! \brief appendFieldString Append one field with QString to query
    //! \param fieldName         Field name
    //! \param value             QString value. It converted to utf8
    //! \return                  Query for subsequential call
    //!
    SvNetQuery &appendFieldString( const QString fieldName, const QString value ) { return appendField( fieldName, value.toUtf8() ); }

    //!
    //! \brief appendFieldData Append one field with binary data to query
    //! \param fieldName       Field name
    //! \param data            Binary data. It converted to hex string representation
    //! \return                Query for subsequential call
    //!
    SvNetQuery &appendFieldData( const QString fieldName, const QByteArray data ) { return appendField( fieldName, data.toHex() ); }
  };






class SvNetClient : public QObject
  {
    Q_OBJECT

    QTimer                *mTimer;           //!< Timer for periodic job
    const int              mTimerPeriod;     //!< Timer period

    static SvNetClient    *mInstance;        //!< Client instance. In application may be only one net client
  protected:
    QNetworkAccessManager *mNetworkManager;  //!< Network manager through witch we connect to global repository

  public:
    explicit SvNetClient( int timerPeriod = 10, QObject *parent = nullptr);

    //!
    //! \brief instance Returns net client instance.
    //! \return         Net client instance
    //!
    //! Before call first instance you must call initInstance to init net client instance.
    static SvNetClient *instance();

    //!
    //! \brief initInstance Init net client instance with client
    //! \param client       Client to setup as net client instance
    //!
    static void         initInstance( SvNetClient *client );
  signals:

  public slots:
    //!
    //! \brief onThreadStarted This slot called on thread started
    //!
    //! Net client works in separate thread. So, some initializations must be
    //! performs in that separate thread. We start that thread and when
    //! it started it call this slot to make initializations.
    //!
    //! When redefine this slot you must call its SvNetClient variant.
    virtual void onThreadStarted();

    //!
    //! \brief onTimer Periodic function. By default we done nothing
    //!
    virtual void onTimer();

  protected:
    //!
    //! \brief request        Form and send net request and wait for reply. Return reply result
    //! \param multiPart      Request
    //! \param networkManager Network manager throught which we access to net
    //! \param url            Url for request
    //! \param timeoutMs      Timeout for reply wait
    //! \return               Reply result or empty for timeout or error
    //!
    QByteArray  request(QHttpMultiPart *multiPart, QUrl url, int timeoutMs = 30000);

    QJsonObject requestObject( QHttpMultiPart *multiPart, QUrl url, int timeoutMs = 30000 );

  };

#endif // SVNETCLIENT_H
