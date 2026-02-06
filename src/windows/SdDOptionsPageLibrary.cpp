#include "SdDOptionsPageLibrary.h"
#include "library/SdLibraryStorage.h"

#include <QSettings>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QFileDialog>

SdDOptionsPageLibrary::SdDOptionsPageLibrary(QWidget *parent) :
  QWidget(parent)
  {
  setWindowTitle( tr("Component library") );

  QSettings s;

  QGridLayout *grid = new QGridLayout();
  QPushButton *but;

  grid->addWidget( new QLabel(tr("Local library path:")), 0, 0 );
  grid->addWidget( mLocalLibraryPath = new QLineEdit( s.value(SDK_LOCAL_LIBRARY_PATH).toString() ), 0, 1 );
  grid->addWidget( but = new QPushButton( tr("Select...") ), 1, 2 );
  connect( but, &QPushButton::clicked, this, [this] () {
    QString str = QFileDialog::getExistingDirectory( this, tr("Local library path"), mLocalLibraryPath->text() );
    if( !str.isEmpty() )
      mLocalLibraryPath->setText( str );
    });


  grid->addWidget( new QLabel(tr("Global storage IP:")), 1, 0 );
  grid->addWidget( mGlobalStorageIp = new QLineEdit(s.value( QStringLiteral(SDK_GLOBAL_STORAGE_IP), SD_DEFAULT_GLOBAL_STORAGE_IP ).toString()), 1, 1 );
  mGlobalStorageIp->setInputMask( "000.000.000.000;_" );
  grid->addWidget( but = new QPushButton( tr("Resync") ), 1, 2 );
  connect( but, &QPushButton::clicked, this, [] () {
    SdLibraryStorage::instance()->globalStorageSyncReset();
    });


  grid->addWidget( new QLabel(tr("Private cloud IP:")), 2, 0 );
  grid->addWidget( mPrivateCloudIp = new QLineEdit(s.value( QStringLiteral(SDK_PRIVATE_CLOUD_IP), SD_DEFAULT_PRIVATE_CLOUD_IP ).toString()), 2, 1 );
  mPrivateCloudIp->setInputMask( "000.000.000.000;_" );
  grid->addWidget( but = new QPushButton( tr("Resync") ), 2, 2 );
  connect( but, &QPushButton::clicked, this, [] () {
    SdLibraryStorage::instance()->privateCloudSyncReset();
    });


  grid->addWidget( new QLabel(tr("Private cloud ID:")), 3, 0 );
  grid->addWidget( mPrivateCloudId = new QLineEdit( SdLibraryStorage::instance()->privateCloudName() ), 3, 1 );
  //Cloud id builds in form SlxCloud-00000000-0000-0000-0000-000000000000
  mPrivateCloudId->setInputMask( "AAAAAAAA-AAAAAAAA-AAAA-AAAA-AAAAAAAAAAAA;#" );
  grid->addWidget( but = new QPushButton( tr("Get new") ), 3, 2 );
  connect( but, &QPushButton::clicked, this, [this] () {
    mPrivateCloudId->setText( SdLibraryStorage::instance()->privateCloudNameNew() );
    SdLibraryStorage::instance()->privateCloudSyncReset();
    });


/*
  grid->addWidget( new QLabel(tr("Enable automatic object upload:")), 1, 0 );
  grid->addWidget( mAutoUpload = new QCheckBox(), 1, 1 );
  mAutoUpload->setChecked( s.value(SDK_UPLOAD_AUTO, false ).toBool() );
  connect( mAutoUpload, &QCheckBox::toggled, this, [] ( bool state ) {
    QSettings s;
    s.setValue( SDK_UPLOAD_AUTO, state );
    });


  grid->addWidget( new QLabel(tr("Author name:")), 2, 0 );
  grid->addWidget( mAuthorName = new QLineEdit(), 2, 1 );
  //mAuthorName->setText( s.value( QStringLiteral(SDK_GLOBAL_AUTHOR), QString()).toString() );
  mAuthorName->setReadOnly( true );
  grid->addWidget( but = new QPushButton( tr("Registration...") ), 2, 2 );
  connect( but, &QPushButton::clicked, this, [this] () {
    SdDRegistation d;
    d.exec();
    //After registration update ip and author name
    QSettings s;
    mServerRepo->setText( s.value( QStringLiteral(SDK_SERVER_REPO) ).toString() );
//    mAuthorName->setText( s.value( QStringLiteral(SDK_GLOBAL_AUTHOR), QString()).toString() );
    });


  grid->addWidget( new QLabel(tr("Objects in library:")), 3, 0 );
  grid->addWidget( mObjectsCount = new QLineEdit(), 3, 1 );
  mObjectsCount->setText( QString::number(SdLibraryStorage::instance()->objectCount()) );
  mObjectsCount->setReadOnly(true);
  grid->addWidget( but = new QPushButton( tr("Rescan") ), 3, 2 );
  connect( but, &QPushButton::clicked, this, [this] () {
    //Reassign library path will restart scan library process
    SdLibraryStorage::instance()->libraryPathSet( SdLibraryStorage::instance()->libraryPath() );
    mObjectsCount->setText( QString::number(SdLibraryStorage::instance()->objectCount()) );
    });
//  grid->addWidget( but = new QPushButton( tr("Select...") ), 1, 2 );
//  connect( but, &QPushButton::clicked, this, [this] () {
//    QString str = QFileDialog::getExistingDirectory( this, tr("Library path"), mLibraryPath->text() );
//    if( !str.isEmpty() )
//      mLibraryPath->setText( str );
//    });





*/

  setLayout( grid );
  }



void SdDOptionsPageLibrary::accept()
  {
  QSettings s;
  if( mLocalLibraryPath->text() != s.value(SDK_LOCAL_LIBRARY_PATH).toString() ) {
    //Library path changed
    SdLibraryStorage::instance()->libraryPathSet( mLocalLibraryPath->text() );
    SdLibraryStorage::instance()->globalStorageSyncReset();
    SdLibraryStorage::instance()->privateCloudSyncReset();
    }

  if( mGlobalStorageIp->text() != s.value( QStringLiteral(SDK_GLOBAL_STORAGE_IP), SD_DEFAULT_GLOBAL_STORAGE_IP ).toString()  ) {
    //Global storage ip changed
    s.setValue( QStringLiteral(SDK_GLOBAL_STORAGE_IP), mGlobalStorageIp->text() );
    SdLibraryStorage::instance()->globalStorageSyncReset();
    }

  if( mPrivateCloudIp->text() != s.value( QStringLiteral(SDK_PRIVATE_CLOUD_IP), SD_DEFAULT_PRIVATE_CLOUD_IP ).toString() ) {
    //Private cloud ip changed
    s.setValue( QStringLiteral(SDK_PRIVATE_CLOUD_IP), mPrivateCloudIp->text() );
    SdLibraryStorage::instance()->privateCloudSyncReset();
    }

  if( mPrivateCloudId->text() != SdLibraryStorage::privateCloudName() ) {
    //Private cloud name changed
    s.setValue( QStringLiteral(SDK_PRIVATE_CLOUD_NAME), mPrivateCloudId->text() );
    SdLibraryStorage::instance()->privateCloudSyncReset();
    }

  }
