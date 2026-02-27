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
#include "SdDHelp.h"
#include "library/SdLibraryStorage.h"
#include "library/SdTcpCborClient.h"

#include <QSettings>
#include <QMessageBox>
#include <QTimer>
#include <QRegularExpressionValidator>
#include <QtConcurrent/QtConcurrentRun>
#include <QCborMap>



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
  if( s.value( QStringLiteral(SDK_GLOBAL_STORAGE_IP) ).toString().isEmpty() )
    s.setValue( QStringLiteral(SDK_GLOBAL_STORAGE_IP), QString(SD_DEFAULT_GLOBAL_STORAGE_IP) );

  // Регулярное выражение для IP4
  QRegularExpression regex(
      "^((25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}"
      "(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$"
  );

  ui->mServer->setText( s.value( QStringLiteral(SDK_GLOBAL_STORAGE_IP) ).toString() );
  ui->mServer->setInputMask( "999.999.999.999;#" );
  ui->mServer->setValidator( new QRegularExpressionValidator(regex, ui->mServer ) );

  ui->mPrivateKey->setText( SdLibraryStorage::authorPrivateKey() );

  connect( ui->mGetStatus, &QPushButton::clicked, this, &SdDRegistation::cmGetStatus );
  connect( ui->mRegistration, &QPushButton::clicked, this, &SdDRegistation::cmRegistration );
  connect( ui->mGeneratePassword, &QPushButton::clicked, this, &SdDRegistation::cmGeneratePrivateKey );
  connect( ui->mClose, &QPushButton::clicked, this, &SdDRegistation::cmClose );
  connect( ui->mPublicName, &QLineEdit::textEdited, this, &SdDRegistation::onEditAuthorName );

  connect( ui->mHelp, &QPushButton::clicked, this, [this] () {
    SdDHelp::help( QString("SdDRegistration.htm"), this );
    } );

  //Check registration status at start
  QTimer::singleShot( 300, this, &SdDRegistation::cmGetStatus );
  }



SdDRegistation::~SdDRegistation()
  {
  delete ui;
  }



//Registration new user
void SdDRegistation::cmRegistration()
  {
  if( ui->mServer->text().isEmpty() ) {
    QMessageBox::warning( this, tr("Warning!"), tr("Enter repository server") );
    return;
    }
  if( ui->mPublicName->text().isEmpty() ) {
    QMessageBox::warning( this, tr("Warning!"), tr("Enter public author name") );
    return;
    }
  if( ui->mEmail->text().isEmpty() ) {
    QMessageBox::warning( this, tr("Warning!"), tr("Enter your email needed to restore key at later") );
    return;
    }
  if( ui->mPrivateKey->text().isEmpty() )
    //Check access to existing user
    QMessageBox::warning( this, tr("Warning!"), tr("Enter private key for your account") );
  else {
    //Store registration data
    QSettings s;
    s.setValue( QStringLiteral(SDK_GLOBAL_STORAGE_IP), ui->mServer->text() );
    s.setValue( QStringLiteral(SDK_PRIVATE_KEY), ui->mPrivateKey->text() );

    ui->mRegistrationStatus->setText( tr("Query server...") );
    //Perform sync with remote storage
    auto f = QtConcurrent::run( SdDRegistation::performRegistration, this );
    }

  }



void SdDRegistation::cmGeneratePrivateKey()
  {
  //Set new private key
  QString privateKey = SdLibraryStorage::authorPrivateKeyNew();
  QSettings s;
  s.setValue( SDK_PRIVATE_KEY, privateKey );
  ui->mPrivateKey->setText( privateKey );

  //Clear all other fields
  ui->mEmail->clear();
  ui->mPublicName->clear();
  }




void SdDRegistation::cmGetStatus()
  {
  if( ui->mServer->text().isEmpty() ) {
    QMessageBox::warning( this, tr("Warning!"), tr("Enter repository server") );
    return;
    }
  if( ui->mPrivateKey->text().isEmpty() )
    //Check access to existing user
    QMessageBox::warning( this, tr("Warning!"), tr("Enter private key for your account") );
  else {
    //Store registration data
    QSettings s;
    s.setValue( QStringLiteral(SDK_GLOBAL_STORAGE_IP), ui->mServer->text() );
    s.setValue( QStringLiteral(SDK_PRIVATE_KEY), ui->mPrivateKey->text() );

    ui->mRegistrationStatus->setText( tr("Query server...") );
    //Perform sync with remote storage
    auto f = QtConcurrent::run( SdDRegistation::performGetStatus, this );
    }
  }









//On edit user name
void SdDRegistation::onEditAuthorName(const QString nm)
  {
  int nstatus;
  if( nm.isEmpty() )
    nstatus = 0;
#ifdef SD_DISABLE_SALI_AUTHOR
  else if( nm.toLower().contains( QString("salix") ) )
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



void SdDRegistation::performRegistration(SdDRegistation *reg)
  {
  QSettings s;
  try {
    SdTcpCborClient client;
    client.openSocket( s.value(SDK_GLOBAL_STORAGE_IP, SD_DEFAULT_GLOBAL_STORAGE_IP).toString(), SD_GLOBAL_STORAGE_PORT );

    //Prepare registration query
    QCborMap map;
    map[SDRM_TYPE]               = SDRM_TYPE_REGISTER;
    map[SDRM_AUTHOR_PUBLIC_KEY]  = SdLibraryStorage::authorPublicKey();
    map[SDRM_AUTHOR_PRIVATE_KEY] = SdLibraryStorage::authorPrivateKey();
    map[SDRM_AUTHOR_NAME]        = reg->ui->mPublicName->text();
    map[SDRM_AUTHOR_EMAIL]       = reg->ui->mEmail->text();

    map = client.transferMap( map );

    if( map[SDRM_TYPE].toInteger() == SDRM_TYPE_INVALID_KEY_PAIR )
      reg->ui->mRegistrationStatus->setText( tr("Author public key and author private key not equals recorded in db.") );
    else if( map[SDRM_TYPE].toInteger() == SDRM_TYPE_INVALID_NAME )
      reg->ui->mRegistrationStatus->setText( tr("Assigned name always used by other author.") );
    else if( map[SDRM_TYPE].toInteger() == SDRM_TYPE_OK )
      reg->ui->mRegistrationStatus->setText( tr("Registration successfull.") );
    else
      reg->ui->mRegistrationStatus->setText( tr("Registration fail. Try later.") );
    }
  catch(const std::exception& e) {
    qDebug() << "Error occured" << e.what();
    reg->ui->mRegistrationStatus->setText( e.what() );
    }
  }




void SdDRegistation::performGetStatus(SdDRegistation *reg)
  {
  QSettings s;
  try {
    SdTcpCborClient client;
    client.openSocket( s.value(SDK_GLOBAL_STORAGE_IP, SD_DEFAULT_GLOBAL_STORAGE_IP).toString(), SD_GLOBAL_STORAGE_PORT );

    //Prepare registration query
    QCborMap map;
    map[SDRM_TYPE]               = SDRM_TYPE_REGISTER_GET;
    map[SDRM_AUTHOR_PUBLIC_KEY]  = SdLibraryStorage::authorPublicKey();
    map[SDRM_AUTHOR_PRIVATE_KEY] = SdLibraryStorage::authorPrivateKey();
    map = client.transferMap( map );

    if( map[SDRM_TYPE].toInteger() == SDRM_TYPE_OK ) {
      reg->ui->mRegistrationStatus->setText( tr("Registered.") );
      reg->ui->mEmail->setText( map[SDRM_AUTHOR_EMAIL].toString() );
      reg->ui->mPublicName->setText( map[SDRM_AUTHOR_NAME].toString() );
      }
    else if( map[SDRM_TYPE].toInteger() == SDRM_TYPE_FAIL )
      reg->ui->mRegistrationStatus->setText( tr("Not registered.") );
    else
      reg->ui->mRegistrationStatus->setText( tr("Registration status fail. Try later.") );
    }
  catch(const std::exception& e) {
    qDebug() << "Error occured" << e.what();
    reg->ui->mRegistrationStatus->setText( e.what() );
    }
  }







