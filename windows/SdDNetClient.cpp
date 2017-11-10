/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/
#include "SdDNetClient.h"
#include "ui_SdDNetClient.h"
#include <QTimer>

SdDNetClient::SdDNetClient(const QString id, QWidget *parent) :
  QDialog(parent),
  mId(id),
  ui(new Ui::SdDNetClient)
  {
  ui->setupUi(this);

  //In an 100ms after creation emit signal
  QTimer::singleShot( 100, this, [this] () {
    emit requestObject( mId );
    } );

  connect( ui->mCancel, &QPushButton::clicked, this, &SdDNetClient::reject );
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




void SdDNetClient::onCompleteRequestObject(bool res)
  {
  done( res ? 1 : 0 );
  }



void SdDNetClient::onProcess(int pos, int range, const QString oper)
  {
  ui->mProgress->setRange( 0, range );
  ui->mProgress->setValue( pos );
  ui->mOperation->setText( oper );
  }
