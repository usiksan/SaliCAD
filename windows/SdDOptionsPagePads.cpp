/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/
#include "SdDOptionsPagePads.h"
#include "SdDGetObject.h"
#include "objects/SdEnvir.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>

SdDOptionsPagePads::SdDOptionsPagePads(QWidget *parent) :
  QWidget(parent)
  {
  QVBoxLayout *mainLayout = new QVBoxLayout();

  //Layout for pad association table name
  QHBoxLayout *nameLayout = new QHBoxLayout();
  //Title
  nameLayout->addWidget( new QLabel(tr("Pad association name:")) );
  //Name
  mAssociationName = new QLineEdit();
  mAssociationName->setReadOnly(true);
  mAssociationName->setText( sdEnvir->mPadStackTitle );
  nameLayout->addWidget( mAssociationName );
  //Select button
  QPushButton *but = new QPushButton( tr("Select...") );
  nameLayout->addWidget( but );
  connect( but, &QPushButton::clicked, this, &SdDOptionsPagePads::onSelect );

  mainLayout->addLayout( nameLayout );

  //Pad association table
  mPadTable = new QTableWidget( this );

  mainLayout->addWidget( mPadTable );
  setLayout( mainLayout );

  updatePinTable();
  }




//Select and load new pad association
void SdDOptionsPagePads::onSelect()
  {
  QString uid = SdDGetObject::getObjectUid( dctPadAssociation, tr("Select default pad association"), this );
  if( !uid.isEmpty() ) {
    sdEnvir->resetPads();
    sdEnvir->mPadStackUid = uid;
    updatePinTable();
    }
  }




void SdDOptionsPagePads::accept()
  {

  }




//Fill pad table with pads associations
void SdDOptionsPagePads::updatePinTable()
  {
  //Clear current table contens
  mPadTable->clear();
  //Table dimensions
  mPadTable->setColumnCount(2);
  mPadTable->setColumnWidth(1,440);
  //Fictiv read pad for load pad stack if it not loaded yet
  sdEnvir->getPad( QString("def") );
  //Check if pad stack present
  if( sdEnvir->mPadStack ) {
    //Setup pads title
    mAssociationName->setText( sdEnvir->mPadStack->getName() );

    //Get pad association map
    SdPadMap map = sdEnvir->mPadStack->getMap();

    //Fill visual table with map data
    mPadTable->setRowCount( map.count() );
    //Table header
    mPadTable->setHorizontalHeaderLabels( {tr("Pin type"), tr("Pad description") } );

    int i = 0;
    for( auto iter = map.cbegin(); iter != map.cend(); iter++ ) {
      mPadTable->setRowHeight(i,25);
      QTableWidgetItem *item;
      mPadTable->setItem( i, 0, item = new QTableWidgetItem(iter.key()) );
      item->setFlags( Qt::ItemIsEnabled );
      mPadTable->setItem( i, 1, item = new QTableWidgetItem(iter.value().description()) );
      item->setFlags( Qt::ItemIsEnabled );
      i++;
      }
    }
  }
