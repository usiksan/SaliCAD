#include "SvNetClient.h"

#include <QDebug>
#include <QThread>
#include <QFuture>
#include <QPromise>
//#include <QtConcurrent/QtConcurrent>
#include <QNetworkRequest>
#include <QHttpMultiPart>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>


SvNetQuery &SvNetQuery::appendField(const QString fieldName, const QByteArray data)
  {
  QHttpPart textPart;
  textPart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("text/plain"));
  textPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant( QStringLiteral("form-data; name=\"%1\"").arg(fieldName)) );
  textPart.setBody(data);

  if( mMultiPart == nullptr )
    mMultiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

  mMultiPart->append( textPart );

  return *this;
  }




SvNetClient *SvNetClient::mInstance;

SvNetClient::SvNetClient(int timerPeriod, QObject *parent)
  : QObject{parent}
  , mTimerPeriod(timerPeriod)
  {

  }




//!
//! \brief instance Returns net client instance.
//! \return         Net client instance
//!
//! Before call first instance you must call initInstance to init net client instance.
SvNetClient *SvNetClient::instance()
  {
  if( mInstance == nullptr )
    qFatal() << "SvNetClient instance called before initInstance";
  return mInstance;
  }





//!
//! \brief initInstance Init net client instance with client
//! \param client       Client to setup as net client instance
//!
void SvNetClient::initInstance(SvNetClient *client)
  {
  if( mInstance != nullptr )
    qFatal() << "Duplicate SvNetClient initInstance";

  mInstance = client;

  QThread *thread = new QThread();
  mInstance->moveToThread( thread );

  connect( thread, &QThread::started, mInstance, &SvNetClient::onThreadStarted );
  thread->start();
  }






//!
//! \brief onThreadStarted This slot called on thread started
//!
//! Net client works in separate thread. So, some initializations must be
//! performs in that separate thread. We start that thread and when
//! it started it call this slot to make initializations.
//!
//! When redefine this slot you must call its SvNetClient variant.
void SvNetClient::onThreadStarted()
  {
  mNetworkManager = new QNetworkAccessManager(this);

  mTimer = new QTimer();
  connect( mTimer, &QTimer::timeout, this, &SvNetClient::onTimer );
  mTimer->start( mTimerPeriod );
  }






//!
//! \brief onTimer Periodic function. By default we done nothing
//!
void SvNetClient::onTimer()
  {

  }




//!
//! \brief request        Form and send net request and wait for reply. Return reply result
//! \param multiPart      Request
//! \param networkManager Network manager throught which we access to net
//! \param url            Url for request
//! \param timeoutMs      Timeout for reply wait
//! \return               Reply result or empty for timeout or error
//!
QByteArray SvNetClient::request(QHttpMultiPart *multiPart, QUrl url, int timeoutMs)
  {
  QNetworkReply *reply = mNetworkManager->post(QNetworkRequest(url), multiPart);
  multiPart->setParent(reply);

  // Используем shared_ptr для безопасного разделения между лямбдами
  auto queryPromise = std::make_shared<QPromise<bool>>();
  QFuture<bool> query = queryPromise->future();
  queryPromise->start();

  QObject::connect(reply, &QNetworkReply::finished, [queryPromise,reply]() {
    if( queryPromise->future().isRunning() ) {
      queryPromise->addResult(reply->error() == QNetworkReply::NoError);
      queryPromise->finish();
      }
    });
  QTimer::singleShot( timeoutMs, [queryPromise,reply]() {
    if( queryPromise->future().isRunning() ) {
      reply->abort();
      queryPromise->addResult(false);
      queryPromise->finish();
      }
    });

  query.waitForFinished();

  bool success = query.result();
  QByteArray result = success ? reply->readAll() : QByteArray();

  reply->deleteLater();
  return result;
  }




QJsonObject SvNetClient::requestObject(QHttpMultiPart *multiPart, QUrl url, int timeoutMs)
  {
  QByteArray ar( request( multiPart, url, timeoutMs ) );
  return ar.isEmpty() ? QJsonObject{} : QJsonDocument::fromJson( ar ).object();
  }

