/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/
#include "SdWSection.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QStringList>

SdWSection::SdWSection(SdSection *s, QWidget *parent) :
  QWidget(parent),
  mSection(s)
  {
  QHBoxLayout *hbox = new QHBoxLayout();
  hbox->addWidget( new QLabel(tr("Name:")) );
  hbox->addWidget( mTitle = new QLabel() );
  QVBoxLayout *vbox = new QVBoxLayout();
  vbox->addLayout( hbox );
  vbox->addWidget( mPinTable = new QTableWidget( 1, 2, this) );
  setLayout( vbox );

  QStringList labels;
  labels << tr("Pin name") << tr("Pin number");
  mPinTable->setHorizontalHeaderLabels( labels );

  }




//Update visual title as in section
void SdWSection::updateTitle()
  {
  mTitle->setText( mSection->getSymbolTitle() );
  }



//Update visual pin assotiation table
void SdWSection::updatePinTable()
  {
  mPinTable->setRowCount(1);
  SdPinAssotiation pins = mSection->getPins();
  for( auto i = pins.constBegin(); i != pins.constEnd(); i++ ) {
    mPinTable->insertRow( mPinTable->rowCount() );
    mPinTable->setItem( mPinTable->rowCount() - 1, 0, new QTableWidgetItem( i.key() ) );
    mPinTable->setItem( mPinTable->rowCount() - 1, 1, new QTableWidgetItem( i.value() ) );
    }
  }

