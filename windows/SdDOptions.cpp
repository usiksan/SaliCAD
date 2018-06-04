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

  //Append option pages
  SdDOptionsPageColors *colors = new SdDOptionsPageColors();
  connect(mButtons, &QDialogButtonBox::accepted, colors, &SdDOptionsPageColors::accept );
  mTabWidget->addTab( colors, tr("Colors") );

  SdDOptionsPageEditors *editors = new SdDOptionsPageEditors();
  connect(mButtons, &QDialogButtonBox::accepted, editors, &SdDOptionsPageEditors::accept );
  mTabWidget->addTab( editors, tr("Editors") );

  setWindowTitle( tr("Options") );
  resize( 700, 500 );
  setModal( true );
  }



SdDOptions::~SdDOptions()
  {

  }

