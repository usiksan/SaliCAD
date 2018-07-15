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

SdDRegistation::SdDRegistation(bool fromHelp, QWidget *parent) :
  QDialog(parent),
  mFromHelp(fromHelp),
  mNameStatus(-1),
  ui(new Ui::SdDRegistation)
  {
  ui->setupUi(this);

  //Fill fields
  QSettings s;
  ui->mServerIP->setText( s.value( QStringLiteral(SDK_SERVER_IP), QString("127.0.0.1")).toString() );
  ui->mName->setText( s.value( QStringLiteral(SDK_GLOBAL_AUTHOR), QString()).toString() );
  onEditAuthorName( ui->mName->text() );
  ui->mMachineKey->setText( s.value( QStringLiteral(SDK_MACHINE_KEY), QString()).toString() );
  ui->mRemain->setText( s.value( QStringLiteral(SDK_REMOTE_REMAIN), QString::number(SD_DEFAULT_DELIVERED_LIMIT) ).toString() );
  ui->mRemain->setReadOnly(true);

  connect( ui->mRegistration, &QPushButton::clicked, this, &SdDRegistation::cmRegistration );
  connect( ui->mAddMachine, &QPushButton::clicked, this, &SdDRegistation::cmAddMachine );
  connect( ui->mClose, &QPushButton::clicked, this, &SdDRegistation::cmClose );
  connect( ui->mName, &QLineEdit::textEdited, this, &SdDRegistation::onEditAuthorName );

  connect( this, &SdDRegistation::doRegistration, sdObjectNetClient, &SdObjectNetClient::doRegistration );
  connect( this, &SdDRegistation::doMachine, sdObjectNetClient, &SdObjectNetClient::doMachine );
  connect( sdObjectNetClient, &SdObjectNetClient::registrationComplete, this, &SdDRegistation::onRegistrationComplete );
  connect( sdObjectNetClient, &SdObjectNetClient::process, this, [this] (const QString msg, bool stop ) {
    Q_UNUSED(stop)
    ui->mProcess->setText(msg);
    });
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
  if( ui->mEmail->text().isEmpty() ) {
    QMessageBox::warning( this, tr("Warning!"), tr("Enter your email needed to resore key at later") );
    return;
    }
  if( ui->mMachineKey->text().isEmpty() )
    //Registration new user
    emit doRegistration( ui->mServerIP->text(), ui->mName->text().toLower(), ui->mEmail->text() );
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
  emit doMachine( ui->mServerIP->text(), ui->mName->text(), ui->mMachineKey->text().toULongLong( nullptr, 32 ) );
  }




//On complete registration
void SdDRegistation::onRegistrationComplete( const QString authorName, const QString email, quint64 key, int remain, int result )
  {
  if( result == SCPE_SUCCESSFULL ) {
    ui->mName->setText( authorName );
    ui->mEmail->setText( email );
    ui->mMachineKey->setText( QString::number( key, 32 ) );
    ui->mRemain->setText( QString::number(remain) );
    }
  else if( result == SCPE_AUTHOR_ALREADY_PRESENT )
    QMessageBox::warning( this, tr("Error!"), tr("This user name already present. Enter another name.") );
  else
    QMessageBox::warning( this, tr("Error!"), tr("Undefined error %1").arg(result) );
  }





//On edit user name
void SdDRegistation::onEditAuthorName(const QString nm)
  {
  int nstatus;
  if( nm.isEmpty() )
    nstatus = 0;
  else if( nm.toLower().contains( QString("sali") ) )
    nstatus = 1;
  else
    nstatus = 2;
  if( mNameStatus != nstatus ) {
    mNameStatus = nstatus;
    if( mNameStatus == 0 )
      ui->mNameTitle->setText( tr("Name (empty):") );
    else if( mNameStatus == 1 )
      ui->mNameTitle->setText( tr("Name (unavailable):") );
    else
      ui->mNameTitle->setText( tr("Name:") );
    ui->mRegistration->setEnabled( mNameStatus == 2 );
    }
  }




//Close dialog
void SdDRegistation::cmClose()
  {
  if( !sdObjectNetClient->isRegistered() ) {
    QMessageBox::warning( this, tr("Warning!"), tr("You not registered. SaliCAD will work in autonom mode. In this mode You can not access global component database. This dialog allowed in later throw Help menu."));
    }
  else {
    if( !mFromHelp )
      QMessageBox::warning( this, tr("Warning!"), tr("This dialog allowed in later throw Help menu."));
    }
  done(1);
  }



