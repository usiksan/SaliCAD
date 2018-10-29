/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Some common params:
    - interface language
*/
#include "SdDOptionsPageCommon.h"

#include <QSettings>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>

SdDOptionsPageCommon::SdDOptionsPageCommon(QWidget *parent) :
  QWidget(parent)
  {
  QSettings s;
  setWindowTitle( tr("Paths") );

  QGridLayout *grid = new QGridLayout();

  grid->addWidget( new QLabel(tr("Interface language:")), 0, 0 );
  grid->addWidget( mLanguage = new QComboBox(), 0, 1 );


  setLayout( grid );
  }

void SdDOptionsPageCommon::accept()
  {

  }
