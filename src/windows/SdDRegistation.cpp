/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Registration dialog.

  Registration is to set the name of user with appropriate key. By this name and key
  server counts object transfered to user.
*/
#include "SdConfig.h"
#include "SdDRegistation.h"
#include "ui_SdDRegistation.h"
#include "objects/SdObjectNetClient.h"
#include "objects/SdTime2x.h"
#include "SdDHelp.h"

#include <QSettings>
#include <QMessageBox>
#include <QRandomGenerator64>
#include <QTimer>

SdDRegistation::SdDRegistation(bool fromHelp, QWidget *parent) :
  QDialog(parent),
  mFromHelp(fromHelp),
  mNameStatus(-1),
  ui(new Ui::SdDRegistation)
  {
  ui->setupUi(this);

  sdObjectNetClient->startSync(false);

  //Fill fields
  QSettings s;
  ui->mServerRepo->setText( s.value( QStringLiteral(SDK_SERVER_REPO), QString(SD_DEFAULT_REPO)).toString() );
  ui->mName->setText( s.value( QStringLiteral(SDK_GLOBAL_AUTHOR), QString()).toString() );
  onEditAuthorName( ui->mName->text() );
  ui->mPassword->setText( s.value( QStringLiteral(SDK_GLOBAL_PASSWORD), QString()).toString() );

  connect( ui->mRegistration, &QPushButton::clicked, this, &SdDRegistation::cmRegistration );
  connect( ui->mGeneratePassword, &QPushButton::clicked, this, &SdDRegistation::cmGeneratePassword );
  connect( ui->mClose, &QPushButton::clicked, this, &SdDRegistation::cmClose );
  connect( ui->mName, &QLineEdit::textEdited, this, &SdDRegistation::onEditAuthorName );

  connect( this, &SdDRegistation::doRegistration, sdObjectNetClient, &SdObjectNetClient::doRegister );
  connect( sdObjectNetClient, &SdObjectNetClient::process, this, [this] (const QString msg, bool stop ) {
    Q_UNUSED(stop)
    ui->mProcess->setText(msg);
    });
  connect( ui->mHelp, &QPushButton::clicked, this, [this] () {
    SdDHelp::help( QString("SdDRegistration.htm"), this );
    } );

  //Check registration status at start
  connect( sdObjectNetClient, &SdObjectNetClient::registerStatus, this, [this] (const QString msg, const QString email ) {
    ui->mRegistrationStatus->setText(msg);
    if( !email.isEmpty() )
      ui->mEmail->setText( email );
    });

  if( s.contains(SDK_GLOBAL_AUTHOR) && s.contains(SDK_GLOBAL_PASSWORD) && s.contains(SDK_SERVER_REPO) ) {
    ui->mEmail->setText( QStringLiteral("email") );
    QTimer::singleShot( 300, this, &SdDRegistation::cmRegistration );
    }
  else
    ui->mRegistrationStatus->setText( tr("Not registered!") );
  }



SdDRegistation::~SdDRegistation()
  {
  delete ui;
  sdObjectNetClient->startSync(true);
  }



//Registration new user
void SdDRegistation::cmRegistration()
  {
  if( ui->mServerRepo->text().isEmpty() ) {
    QMessageBox::warning( this, tr("Warning!"), tr("Enter repository server") );
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
  if( ui->mPassword->text().isEmpty() )
    //Check access to existing user
    QMessageBox::warning( this, tr("Warning!"), tr("Enter password for your name") );
  else {
    //Store registration data
    QSettings s;
    s.setValue( QStringLiteral(SDK_SERVER_REPO), ui->mServerRepo->text() );
    s.setValue( QStringLiteral(SDK_GLOBAL_AUTHOR), ui->mName->text() );
    s.setValue( QStringLiteral(SDK_GLOBAL_PASSWORD), ui->mPassword->text() );

    //Registration new user
    emit doRegistration( ui->mServerRepo->text(), ui->mName->text().toLower(), ui->mPassword->text(), ui->mEmail->text() );
    }
  }




//!
//! \brief cmGeneratePassword Generate new password
//!
void SdDRegistation::cmGeneratePassword()
  {
  static QRandomGenerator64 gen;
  static bool seed;
  if( !seed ) {
    gen.seed( SdTime2x::current() );
    seed = true;
    }
  qint64 key = gen.generate();
  ui->mPassword->setText( QString::number(key,16) );
  }







//On edit user name
void SdDRegistation::onEditAuthorName(const QString nm)
  {
  int nstatus;
  if( nm.isEmpty() )
    nstatus = 0;
#ifdef SD_DISABLE_SALI_AUTHOR
  else if( nm.toLower().contains( QString("sali") ) )
    nstatus = 1;
#endif
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
    QMessageBox::warning( this, tr("Warning!"), tr("You not registered. SaliCAD will work in autonom mode. In this mode You can not access global component database. This dialog allowed in later with Help menu."));
    }
  else {
    if( !mFromHelp )
      QMessageBox::warning( this, tr("Warning!"), tr("This dialog allowed in later with Help menu."));
    }
  done(1);
  }



