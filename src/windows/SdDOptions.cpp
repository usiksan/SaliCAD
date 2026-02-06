/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Dialog with tab pages of options
*/

#include "SdDOptions.h"
#include "SdDOptionsPageCommon.h"
#include "SdDOptionsPageColors.h"
#include "SdDOptionsPageEditors.h"
#include "SdDOptionsPagePath.h"
#include "SdDOptionsPageLibrary.h"
#include "SdDHelp.h"
#include "library/SdLibraryStorage.h"

#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>

SdDOptions::SdDOptions(QWidget *parent) :
  QDialog( parent, Qt::Dialog | Qt::WindowTitleHint )
  {
  //We disable library manager
  SdLibraryStorage::instance()->libraryPeriodicBreakResume( true );

  QLabel *title = new QLabel( tr("Options"), this );
  mTabWidget = new QTabWidget( this );
  mButtons = new QDialogButtonBox(QDialogButtonBox::Ok|QDialogButtonBox::Help);

  connect(mButtons, &QDialogButtonBox::accepted, this, &SdDOptions::accept );
  connect(mButtons, &QDialogButtonBox::rejected, this, &SdDOptions::reject );
  connect( mButtons->button(QDialogButtonBox::Help), &QPushButton::clicked, this, [this] () {
    SdDHelp::help( QString("SdDOptions.htm"), this );
    } );

  QVBoxLayout *mainLayout = new QVBoxLayout;
  mainLayout->addWidget( title );
  mainLayout->addWidget(mTabWidget);
  mainLayout->addWidget(mButtons);
  setLayout(mainLayout);

  //---------------------------
  //Append option pages

  //Common params
  SdDOptionsPageCommon *common = new SdDOptionsPageCommon();
  connect(mButtons, &QDialogButtonBox::accepted, common, &SdDOptionsPageCommon::accept );
  mTabWidget->addTab( common, tr("Common") );

  //Colors page
  SdDOptionsPageColors *colors = new SdDOptionsPageColors();
  connect(mButtons, &QDialogButtonBox::accepted, colors, &SdDOptionsPageColors::accept );
  mTabWidget->addTab( colors, tr("Colors") );

  //Path page
  SdDOptionsPagePath *paths = new SdDOptionsPagePath();
  connect(mButtons, &QDialogButtonBox::accepted, paths, &SdDOptionsPagePath::accept );
  mTabWidget->addTab( paths, tr("Paths") );

  //Editors page
  SdDOptionsPageEditors *editors = new SdDOptionsPageEditors();
  connect(mButtons, &QDialogButtonBox::accepted, editors, &SdDOptionsPageEditors::accept );
  mTabWidget->addTab( editors, tr("Editors") );

  //Library page
  SdDOptionsPageLibrary *library = new SdDOptionsPageLibrary();
  connect(mButtons, &QDialogButtonBox::accepted, library, &SdDOptionsPageLibrary::accept );
  mTabWidget->addTab( library, tr("Library") );


  setWindowTitle( tr("Options") );
  resize( 700, 500 );
  setModal( true );
  }



SdDOptions::~SdDOptions()
  {
  //Resume library manager
  SdLibraryStorage::instance()->libraryPeriodicBreakResume( false );
  }

