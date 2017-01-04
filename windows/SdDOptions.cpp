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
#include <QVBoxLayout>
#include <QLabel>

SdDOptions::SdDOptions(QWidget *parent) :
  QDialog( parent, Qt::Dialog | Qt::WindowTitleHint )
  {
  QLabel *title = new QLabel( tr("Options"), this );
  mTabWidget = new QTabWidget( this );
  mButtons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

  connect(mButtons, SIGNAL(accepted()), this, SLOT(accept()));
  connect(mButtons, SIGNAL(rejected()), this, SLOT(reject()));

  QVBoxLayout *mainLayout = new QVBoxLayout;
  mainLayout->addWidget( title );
  mainLayout->addWidget(mTabWidget);
  mainLayout->addWidget(mButtons);
  setLayout(mainLayout);

  setWindowTitle( tr("Options") );
  resize( 700, 500 );
  setModal( true );
  }

SdDOptions::~SdDOptions()
  {

  }

