#include "SdDPads.h"
#include "ui_SdDPads.h"

#include "objects/SdGraphPartImp.h"
#include "objects/SdUtil.h"
#include "objects/SdObjectFactory.h"
#include "SdDGetObject.h"

#include <QTableWidgetItem>
#include <QMessageBox>

SdDPads::SdDPads(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::SdDPads)
  {
  ui->setupUi(this);
  }

SdDPads::~SdDPads()
  {
  delete ui;
  }

void SdDPads::changeEvent(QEvent *e)
  {
  QDialog::changeEvent(e);
  switch (e->type()) {
    case QEvent::LanguageChange:
      ui->retranslateUi(this);
      break;
    default:
      break;
    }
  }




void SdDPads::updatePinTable()
  {
  int curRow = ui->mPadTable->currentRow();
  //int curCol
  //Clear current table contens
  ui->mPadTable->clear();
  //Table dimensions
  ui->mPadTable->setColumnCount(2);
  ui->mPadTable->setRowCount( mMap.count() );
  //Table header
  ui->mPadTable->setHorizontalHeaderLabels( {tr("Pin type"), tr("Pad description") } );

  int i = 0;
  for( auto iter = mMap.cbegin(); iter != mMap.cend(); iter++ ) {
    QTableWidgetItem *item;
    ui->mPadTable->setItem( i, 0, item = new QTableWidgetItem(iter.key()) );
    item->setData( Qt::UserRole, iter.key() );
    ui->mPadTable->setItem( i, 1, item = new QTableWidgetItem(iter.value().description()) );
    item->setFlags( Qt::ItemIsEnabled );
    if( iter.value().isEmpty() )
      item->setBackgroundColor( QColor(Qt::yellow).lighter() );
    i++;
    }

  ui->mPadTable->setCurrentCell( curRow, 0 );
  }





//Append to association table full list of used in plate pins
void SdDPads::cmAccumUsedPins()
  {
  //Scan plate and accum used in part implementations used pads
  mPlate->forEachConst( dctPartImp, [this] (SdObject *obj) -> bool {
    SdGraphPartImp *imp = dynamic_cast<SdGraphPartImp*>(obj);
    if( imp != nullptr )
      imp->accumUsedPins( mMap );
    });

  //Update visual pin table
  updatePinTable();
  }




//Append single empty pin to visual table. Appending to mMap will after pinType enter
void SdDPads::cmAppendPin()
  {
  //Append new empty pin association
  int row = ui->mPadTable->rowCount();
  ui->mPadTable->insertRow( row );
  ui->mPadTable->setItem( row, 0, new QTableWidgetItem() );
  QTableWidgetItem *item;
  ui->mPadTable->setItem( row, 1, item = new QTableWidgetItem() );
  item->setFlags( Qt::ItemIsEnabled );
  }




//Delete single pin from visual table and mMap
void SdDPads::cmDeletePin()
  {
  int row = ui->mPadTable->currentRow();
  QTableWidgetItem *item = ui->mPadTable->item(row, 0);
  if( item == nullptr )
    QMessageBox::warning( this, tr("Error"), tr("Not selected pin to delete. Select one and retry.") );
  else {
    //Retrive key (pinType) from padTable
    QString key = item->data(Qt::UserRole).toString();
    if( !key.isEmpty() ) {
      //PinType present, ask question to delete pin
      if( QMessageBox::question( this, tr("Warning"), tr("Are you sure to delete pin \"%1\"").arg(key) ) == QMessageBox::Yes )
        //Answer Yes - delete pin from map
        mMap.remove( key );
      else return;
      }
    //Delete pin from table
    ui->mPadTable->removeRow(row);
    }
  }





//Delete all pins (clear pin-to-pad association table)
void SdDPads::cmDeleteAllPins()
  {
  if( QMessageBox::question( this, tr("Warning"), tr("Are you sure to delete all pins?") ) == QMessageBox::Yes ) {
    //Answer yes - delete all pins
    mMap.clear();
    //Update visual pin table
    updatePinTable();
    }
  }




//Replace all association in table with empty pad (default)
void SdDPads::cmAssociationClear()
  {
  //For each association in table clear pad description
  for( auto iter = mMap.begin(); iter != mMap.end(); iter++ )
    iter.value().clear();
  //Update visual pin table
  updatePinTable();
  }




//Save pin-to-pad association table in local library
void SdDPads::cmAssociationSave()
  {
  QString name = ui->mAssociationName->text();
  if( name.isEmpty() )
    QMessageBox::warning( this, tr("Error"), tr("Can't save because name not entered. Enter name for this association table and retry save."));
  else {
    //Object for saving
    SdPadAssociation assoc( mMap, name, mPlate->getDefaultAuthor() );
    //Creating header
    SdLibraryHeader hdr;
    hdr.mName = name;
    hdr.mType = assoc.getType();
    hdr.mAuthor = mPlate->getDefaultAuthor();
    //hdr.mTag =
    hdr.mTime = SdUtil::getTime2000();
    hdr.mClass = assoc.getClass();
    //Store in library
    SdObjectFactory::insertObject( &assoc, hdr, assoc.write() );
    QMessageBox::information( this, tr("Info"), tr("Pad association saved") );
    }
  }




//Assign associations from loaded table
void SdDPads::cmAssociationUse()
  {
  //Assign associations from loaded table
  SdPadAssociation *assoc = only<SdPadAssociation>( SdDGetObject::getObject(dctPadAssociation, tr("Select pad association table"), this) );
  if( assoc != nullptr ) {
    //Source table loaded - parse
    SdPadMap dst;
    for( auto iter = mMap.cbegin(); iter != mMap.cend(); iter++ )
      if( assoc->contains(iter.key()) )
        dst.insert( iter.key(), assoc->pin(iter.key()) );
      else
        dst.insert( iter.key(), iter.value() );
    //Replace mMap with parsed result
    mMap = dst;
    //Update visual pin table
    updatePinTable();
    }
  }





//Replace pin-to-pad association table with loaded table
void SdDPads::cmAssociationLoad()
  {
  //Replace table with loaded
  SdPadAssociation *assoc = only<SdPadAssociation>( SdDGetObject::getObject(dctPadAssociation, tr("Select pad association table"), this) );
  if( assoc != nullptr ) {
    ui->mAssociationName->setText( assoc->getName() );
    mMap = assoc->getMap();
    //Update visual pin table
    updatePinTable();
    }
  }




void SdDPads::cmCellEditComplete(int row, int column)
  {

  }

void SdDPads::cmCellClicked(int row, int column)
  {

  }



