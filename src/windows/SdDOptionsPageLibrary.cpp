#include "SdDOptionsPageLibrary.h"
#include "SdDRegistation.h"
#include "objects/SdEnvir.h"
#include "library/SvDir.h"
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
  grid->addWidget( mServerIP = new QLineEdit(), 0, 1 );
  mServerIP->setText( s.value( QStringLiteral(SDK_SERVER_IP) ).toString() );
  mServerIP->setReadOnly(true);
  grid->addWidget( but = new QPushButton( tr("Resync") ), 0, 2 );
  connect( but, &QPushButton::clicked, this, [] () {
    QSettings s;
    s.setValue( SDK_LOCAL_SYNC, 0 );
    s.setValue( SDK_REMOTE_SYNC, 0 );
    //s.value( SDK_SERVER_IP ).toString();
    });


  grid->addWidget( new QLabel(tr("Author name:")), 1, 0 );
  grid->addWidget( mAuthorName = new QLineEdit(), 1, 1 );
  mAuthorName->setText( s.value( QStringLiteral(SDK_GLOBAL_AUTHOR), QString()).toString() );
  mAuthorName->setReadOnly( true );
  grid->addWidget( but = new QPushButton( tr("Registration...") ), 1, 2 );
  connect( but, &QPushButton::clicked, this, [this] () {
    SdDRegistation d;
    d.exec();
    //After registration update ip and author name
    QSettings s;
    mServerIP->setText( s.value( QStringLiteral(SDK_SERVER_IP) ).toString() );
    mAuthorName->setText( s.value( QStringLiteral(SDK_GLOBAL_AUTHOR), QString()).toString() );
    });


  grid->addWidget( new QLabel(tr("Objects in library:")), 2, 0 );
  grid->addWidget( mObjectsCount = new QLineEdit(), 2, 1 );
  mObjectsCount->setText( QString::number(sdLibraryStorage.objectCount()) );
  mObjectsCount->setReadOnly(true);
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
