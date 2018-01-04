/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/
#include "SdConfig.h"
#include "SdDRegistation.h"
#include "ui_SdDRegistation.h"
#include "objects/SdObjectNetClient.h"

#include <QSettings>
#include <QMessageBox>

SdDRegistation::SdDRegistation(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::SdDRegistation)
  {
  ui->setupUi(this);

  //Fill fields
  QSettings s;
  ui->mServerIP->setText( s.value( QStringLiteral(SDK_SERVER_IP), QString("127.0.0.1")).toString() );
  ui->mName->setText( s.value( QStringLiteral(SDK_GLOBAL_AUTHOR), QString()).toString() );
  ui->mMachineKey->setText( s.value( QStringLiteral(SDK_MACHINE_KEY), QString()).toString() );
  ui->mLimit->setText( QString::number(SD_DEFAULT_DELIVERED_LIMIT) );
  ui->mLimit->setReadOnly(true);
  ui->mLoaded->setText( QString("0") );
  ui->mLoaded->setReadOnly( true );

  connect( ui->mRegistration, &QPushButton::clicked, this, &SdDRegistation::cmRegistration );
  connect( ui->mAddMachine, &QPushButton::clicked, this, &SdDRegistation::cmAddMachine );
  connect( ui->mClose, &QPushButton::clicked, this, &SdDRegistation::cmClose );

  connect( this, &SdDRegistation::doRegistration, sdObjectNetClient, &SdObjectNetClient::doRegistration );
  connect( this, &SdDRegistation::doMachine, sdObjectNetClient, &SdObjectNetClient::doMachine );
  connect( sdObjectNetClient, &SdObjectNetClient::registrationComplete, this, &SdDRegistation::onRegistrationComplete );
  }



SdDRegistation::~SdDRegistation()
  {
  delete ui;
  }



//Registration new user
void SdDRegistation::cmRegistration()
  {
  if( ui->mServerIP->text().isEmpty() ) {
    QMessageBox::warning( this, tr("Warning!"), tr("Enter server ip") );
    return;
    }
  if( ui->mName->text().isEmpty() ) {
    QMessageBox::warning( this, tr("Warning!"), tr("Enter user name") );
    return;
    }
  if( ui->mDescription->text().isEmpty() ) {
    QMessageBox::warning( this, tr("Warning!"), tr("Enter user description") );
    return;
    }
  if( ui->mMachineKey->text().isEmpty() )
    //Registration new user
    emit doRegistration( ui->mServerIP->text(), ui->mName->text(), ui->mDescription->text() );
  else
    //Check access to existing user
    QMessageBox::warning( this, tr("Warning!"), tr("To start new user registration clear machine key field") );
  }




//Add to user new machine
void SdDRegistation::cmAddMachine()
  {
  if( ui->mServerIP->text().isEmpty() ) {
    QMessageBox::warning( this, tr("Warning!"), tr("Enter server ip") );
    return;
    }
  if( ui->mName->text().isEmpty() ) {
    QMessageBox::warning( this, tr("Warning!"), tr("Enter user name") );
    return;
    }
  if( ui->mMachineKey->text().isEmpty() ) {
    QMessageBox::warning( this, tr("Error!"), tr("You must enter existing machine key."));
    return;
    }
  //Registration new machine
  emit doMachine( ui->mServerIP->text(), ui->mName->text(), ui->mMachineKey->text() );
  }




//On complete registration
void SdDRegistation::onRegistrationComplete(const QString authorName, const QString descr, const QString key, int limit, int delivered, int result)
  {
  if( result == SCPE_SUCCESSFULL ) {
    ui->mName->setText( authorName );
    ui->mDescription->setText( descr );
    ui->mMachineKey->setText( key );
    ui->mLimit->setText( QString::number(limit) );
    ui->mLoaded->setText( QString::number(delivered) );
    }
  else if( result == SCPE_AUTHOR_ALREADY_PRESENT )
    QMessageBox::warning( this, tr("Error!"), tr("This user name already present. Enter another name.") );
  else if( result == SCPE_REGISTER_FAIL )
    QMessageBox::warning( this, tr("Error!"), tr("Registration failure") );
  else
    QMessageBox::warning( this, tr("Error!"), tr("Undefined error %1").arg(result) );
  }




//Close dialog
void SdDRegistation::cmClose()
  {
  if( !sdObjectNetClient->isRegistered() ) {
    QMessageBox::warning( this, tr("Warning!"), tr("You not registered. SaliCAD will work in autonom mode. In this mode You can not access global component database. This dialog allowed in later throw Help menu."));
    }
  else {
    QMessageBox::warning( this, tr("Warning!"), tr("This dialog allowed in later throw Help menu."));
    }
  done(1);
  }



