/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/

#include "SdDOptions.h"
#include "SdDOptionsPageColors.h"
#include "SdDOptionsPageEditors.h"
#include "SdDOptionsPagePads.h"
#include "SdDOptionsPagePath.h"
#include <QVBoxLayout>
#include <QLabel>

SdDOptions::SdDOptions(QWidget *parent) :
  QDialog( parent, Qt::Dialog | Qt::WindowTitleHint )
  {
  QLabel *title = new QLabel( tr("Options"), this );
  mTabWidget = new QTabWidget( this );
  mButtons = new QDialogButtonBox(QDialogButtonBox::Ok);

  connect(mButtons, &QDialogButtonBox::accepted, this, &SdDOptions::accept );
  connect(mButtons, &QDialogButtonBox::rejected, this, &SdDOptions::reject );

  QVBoxLayout *mainLayout = new QVBoxLayout;
  mainLayout->addWidget( title );
  mainLayout->addWidget(mTabWidget);
  mainLayout->addWidget(mButtons);
  setLayout(mainLayout);

  //---------------------------
  //Append option pages

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

  //Pads page
  SdDOptionsPagePads *pads = new SdDOptionsPagePads();
  connect(mButtons, &QDialogButtonBox::accepted, pads, &SdDOptionsPagePads::accept );
  mTabWidget->addTab( pads, tr("Pads") );

  setWindowTitle( tr("Options") );
  resize( 700, 500 );
  setModal( true );
  }



SdDOptions::~SdDOptions()
  {

  }

