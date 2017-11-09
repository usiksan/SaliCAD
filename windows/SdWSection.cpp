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
#include "objects/SdProjectItem.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QStringList>

SdWSection::SdWSection(bool anotherAuthor, SdSection *s, QWidget *parent) :
  QWidget(parent),
  mSection(s)
  {
  QHBoxLayout *hbox = new QHBoxLayout();
  hbox->addWidget( new QLabel(tr("Name:")) );
  hbox->addWidget( mTitle = new QLabel(s->getTitle()) );
  QVBoxLayout *vbox = new QVBoxLayout();
  vbox->addLayout( hbox );
  vbox->addWidget( mPinTable = new QTableWidget( 1, 2, this) );
  setLayout( vbox );

  QStringList labels;
  labels << tr("Pin name") << tr("Pin number");
  mPinTable->setHorizontalHeaderLabels( labels );
  updatePinTable( anotherAuthor );

  if( !anotherAuthor )
    connect( mPinTable, &QTableWidget::cellChanged, this, &SdWSection::onPinEditFinish );
  }




//Update visual title as in section
void SdWSection::updateTitle()
  {
  mTitle->setText( mSection->getTitle() );
  }



//Update visual pin assotiation table
void SdWSection::updatePinTable(bool anotherAuthor)
  {
  mPinTable->setSortingEnabled(false);
  mPinTable->setRowCount(0);
  SdPinAssotiation pins = mSection->getPins();
  for( auto i = pins.constBegin(); i != pins.constEnd(); i++ ) {
    mPinTable->insertRow( mPinTable->rowCount() );
    mPinTable->setItem( mPinTable->rowCount() - 1, 0, new QTableWidgetItem( i.key() ) );
    mPinTable->item( mPinTable->rowCount() - 1, 0 )->setFlags( Qt::NoItemFlags );
    mPinTable->setItem( mPinTable->rowCount() - 1, 1, new QTableWidgetItem( i.value() ) );
    //If another author then disable edit function
    if( anotherAuthor )
      mPinTable->item( mPinTable->rowCount() - 1, 1 )->setFlags( Qt::NoItemFlags );
    }
  }




//On pin edit finish
void SdWSection::onPinEditFinish(int row, int column)
  {
  Q_UNUSED(column)
  mSection->setPinNumber( mPinTable->item( row, 0 )->text(), mPinTable->item( row, 1 )->text() );
  }

