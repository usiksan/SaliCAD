#include "SdDOptionsPageLibrary.h"
#include "SdDRegistation.h"
#include "objects/SdEnvir.h"
#include "SvLib/SvDir.h"
#include "library/SdLibraryStorage.h"

#include <QSettings>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>

SdDOptionsPageLibrary::SdDOptionsPageLibrary(QWidget *parent) :
  QWidget(parent)
  {
  setWindowTitle( tr("Remote library") );

  QSettings s;

  QGridLayout *grid = new QGridLayout();
  QPushButton *but;

  grid->addWidget( new QLabel(tr("Remote library IP:")), 0, 0 );
  grid->addWidget( mServerRepo = new QLineEdit(), 0, 1 );
  mServerRepo->setText( s.value( QStringLiteral(SDK_SERVER_REPO) ).toString() );
  mServerRepo->setReadOnly(true);
  grid->addWidget( but = new QPushButton( tr("Resync") ), 0, 2 );
  connect( but, &QPushButton::clicked, this, [] () {
    QSettings s;
    s.setValue( SDK_REMOTE_SYNC, 1 );
    });

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


//  grid->addWidget( new QLabel(tr("Library path:")), 1, 0 );
//  grid->addWidget( mLibraryPath = new QLineEdit(), 1, 1 );
//  mLibraryPath->setText( sdEnvir->mLibraryPath );
//  grid->addWidget( but = new QPushButton( tr("Select...") ), 1, 2 );
//  connect( but, &QPushButton::clicked, this, [this] () {
//    QString str = QFileDialog::getExistingDirectory( this, tr("Library path"), mLibraryPath->text() );
//    if( !str.isEmpty() )
//      mLibraryPath->setText( str );
//    });





  setLayout( grid );
  }
