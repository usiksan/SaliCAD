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
#include "SvLib/SvTime2x.h"
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

  //Fill fields
  QSettings s;
  //Check and setup default values
  if( s.value( QStringLiteral(SDK_SERVER_REPO) ).toString().isEmpty() )
    s.setValue( QStringLiteral(SDK_SERVER_REPO), QString(SD_DEFAULT_REPO) );

  if( s.value( QStringLiteral(SDK_PRIVATE_KEY) ).toString().isEmpty() )
    s.setValue( QStringLiteral(SDK_PRIVATE_KEY), generatePrivateKey() );

  ui->mServerRepo->setText( s.value( QStringLiteral(SDK_SERVER_REPO), QString(SD_DEFAULT_REPO)).toString() );
  //ui->mName->setText( s.value( QStringLiteral(SDK_GLOBAL_AUTHOR), QString()).toString() );
  onEditAuthorName( ui->mName->text() );
  ui->mPassword->setText( s.value( QStringLiteral(SDK_PRIVATE_KEY), QString()).toString() );

  connect( ui->mRegistration, &QPushButton::clicked, this, &SdDRegistation::cmRegistration );
  connect( ui->mGeneratePassword, &QPushButton::clicked, this, &SdDRegistation::cmGeneratePassword );
  connect( ui->mClose, &QPushButton::clicked, this, &SdDRegistation::cmClose );
  connect( ui->mName, &QLineEdit::textEdited, this, &SdDRegistation::onEditAuthorName );

//  connect( this, &SdDRegistation::doRegistration, SdObjectNetClient::instance(), &SdObjectNetClient::doRegister );
  connect( ui->mHelp, &QPushButton::clicked, this, [this] () {
    SdDHelp::help( QString("SdDRegistration.htm"), this );
    } );

  //Check registration status at start
//  connect( SdObjectNetClient::instance(), &SdObjectNetClient::registerStatus, this, [this] ( int success, const QString msg, const QString email ) {
//    ui->mRegistrationStatus->setText(msg);
//    if( !email.isEmpty() )
//      ui->mEmail->setText( email );
//    });

  // if( s.contains(SDK_GLOBAL_AUTHOR) && s.contains(SDK_GLOBAL_PASSWORD) && s.contains(SDK_SERVER_REPO) ) {
  //   ui->mEmail->setText( QStringLiteral("email") );
  //   QTimer::singleShot( 300, this, &SdDRegistation::cmRegistration );
  //   }
  // else {
  //   ui->mRegistrationStatus->setText( tr("Not registered!") );
  //   ui->mAutomaticUpload->setChecked( false );
  //   ui->mAutomaticUpload->setEnabled( false );
  //   }
  }



SdDRegistation::~SdDRegistation()
  {
  delete ui;
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
    QMessageBox::warning( this, tr("Warning!"), tr("Enter private key for your account") );
  else {
    //Store registration data
    QSettings s;
    s.setValue( QStringLiteral(SDK_SERVER_REPO), ui->mServerRepo->text() );
    //s.setValue( QStringLiteral(SDK_GLOBAL_AUTHOR), ui->mName->text() );
    s.setValue( QStringLiteral(SDK_PRIVATE_KEY), ui->mPassword->text() );

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
    gen.seed( SvTime2x::current() );
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
  else if( nm == QStringLiteral("Anonymous") )
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
  // QSettings s;
  // if( !SdObjectNetClient::instance()->isRegistered() ) {
  //   QMessageBox::warning( this, tr("Warning!"), tr("You not registered. You will not be able to save your components in a global repository and share them with society, but you will be able to use components created by others. This dialog allowed in later with Help menu."));
  //   if( s.value( QStringLiteral(SDK_GLOBAL_AUTHOR), QString()).toString().isEmpty() )
  //     //Create default author name
  //     s.setValue( QStringLiteral(SDK_GLOBAL_AUTHOR), QStringLiteral("Anonymous") );
  //   }
  // else {
  //   if( !mFromHelp )
  //     QMessageBox::warning( this, tr("Warning!"), tr("This dialog allowed in later with Help menu."));
  //   }
  // s.setValue( QStringLiteral(SDK_UPLOAD_AUTO), ui->mAutomaticUpload->isChecked() );
  done(1);
  }




QString SdDRegistation::generatePrivateKey()
  {
  static QRandomGenerator64 gen;
  static bool seed;
  if( !seed ) {
    gen.seed( SvTime2x::current() );
    seed = true;
    }
  QString firstPart( QString::number( gen.generate() | 0x8000000000000000, 32 ) + QString::number( gen.generate() | 0x8000000000000000, 32 ) );
  return "User-" + firstPart.mid( 1, 6 ) + "-" + firstPart.mid( 7, 6 ) + "-" + firstPart.mid( 13, 6 ) + "-" + firstPart.mid( 19, 6 );
  }



