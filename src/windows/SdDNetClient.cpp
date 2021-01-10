/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Dialog which show process of load object from remote repository
*/
#include "SdDNetClient.h"
#include "ui_SdDNetClient.h"
#include "objects/SdObjectNetClient.h"
#include <QTimer>
#include <QMessageBox>



SdDNetClient::SdDNetClient(const QString id, QWidget *parent) :
  QDialog(parent),
  mId(id),
  ui(new Ui::SdDNetClient)
  {
  ui->setupUi(this);
  ui->mOperation->setText( id );

  //In an 100ms after creation emit signal
  QTimer::singleShot( 100, this, [this] () {
    emit requestObject( mId );
    } );

  //In an 10sec after creation close dialog with fail
  QTimer::singleShot( 10000, this, [this] () {
    done(0);
    });

  connect( ui->mCancel, &QPushButton::clicked, this, &SdDNetClient::reject );
  connect( this, &SdDNetClient::requestObject, sdObjectNetClient, &SdObjectNetClient::doObject );
  connect( sdObjectNetClient, &SdObjectNetClient::objectComplete, this, &SdDNetClient::onCompleteRequestObject );
  }




SdDNetClient::~SdDNetClient()
  {
  delete ui;
  }




//Get object from remote server
bool SdDNetClient::getObject(QWidget *parent, const QString id)
  {
  SdDNetClient dlg( id, parent );
  return dlg.exec() != 0;
  }




void SdDNetClient::onCompleteRequestObject( int result, int remain )
  {
//  if( result == SCPE_SUCCESSFULL )
//    done(1);
//  else {
//    QMessageBox::warning( this, tr("Error!"), tr("Request result %1, objects remain %2").arg(result).arg(remain) );
//    done(0);
//    }
  }



