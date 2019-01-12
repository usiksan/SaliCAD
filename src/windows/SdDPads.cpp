#include "SdDPads.h"
#include "ui_SdDPads.h"

#include "objects/SdGraphPartImp.h"
#include "objects/SdUtil.h"
#include "objects/SdObjectFactory.h"
#include "objects/SdTime2x.h"
#include "objects/SdEnvir.h"
#include "SdDGetObject.h"
#include "SdDPadMaster.h"
#include "SdDHelp.h"

#include <QTableWidgetItem>
#include <QMessageBox>


#define ACTUAL_PIN Qt::UserRole


SdDPads::SdDPads(SdPItemPlate *plate, const QString associationName, SdPadMap map, QWidget *parent) :
  QDialog(parent),
  mMap(map),
  mPlate(plate),
  ui(new Ui::SdDPads)
  {
  ui->setupUi(this);

  //Setup association name
  ui->mAssociationName->setText( associationName );

  //Connect signals from buttons
  connect( ui->mAccumUsedPins,    &QPushButton::clicked, this, &SdDPads::cmAccumUsedPins );
  ui->mAccumUsedPins->setDisabled( mPlate == nullptr );
  connect( ui->mAppendPin,        &QPushButton::clicked, this, &SdDPads::cmAppendPin );
  connect( ui->mDeletePin,        &QPushButton::clicked, this, &SdDPads::cmDeletePin );
  connect( ui->mDeleteAllPins,    &QPushButton::clicked, this, &SdDPads::cmDeleteAllPins );
  connect( ui->mAssociationClear, &QPushButton::clicked, this, &SdDPads::cmAssociationClear );
  connect( ui->mAssociationLoad,  &QPushButton::clicked, this, &SdDPads::cmAssociationLoad );
  connect( ui->mAssociationSave,  &QPushButton::clicked, this, &SdDPads::cmAssociationSave );
  connect( ui->mAssociationUse,   &QPushButton::clicked, this, &SdDPads::cmAssociationUse );
  //Connect signals from table
  connect( ui->mPadTable, &QTableWidget::cellClicked, this, &SdDPads::cmCellClicked );

  connect( ui->buttonBox->button(QDialogButtonBox::Help), &QPushButton::clicked, this, [this] () {
    SdDHelp::help( QString("SdDPads.htm"), this );
    } );

  updatePinTable();
  }





SdDPads::~SdDPads()
  {
  delete ui;
  }




QString SdDPads::getAssociationName() const
  {
  return ui->mAssociationName->text();
  }



//Retrive current selected pin from association table
QString SdDPads::getCurrentPin() const
  {
  int index = ui->mPadTable->currentRow();
  if( index >= 0 )
    return ui->mPadTable->item( index, 0 )->text();
  return QString();
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
  disconnect( ui->mPadTable, &QTableWidget::cellChanged, this, &SdDPads::cmCellEditComplete );
  int curRow = ui->mPadTable->currentRow();
  //int curCol
  //Clear current table contents
  ui->mPadTable->clear();
  //Table dimensions
  ui->mPadTable->setColumnCount(2);
  ui->mPadTable->setColumnWidth(1,440);
  ui->mPadTable->setRowCount( mMap.count() );
  //Table header
  ui->mPadTable->setHorizontalHeaderLabels( {tr("Pin type"), tr("Pad description") } );

  int i = 0;
  for( auto iter = mMap.cbegin(); iter != mMap.cend(); iter++ ) {
    ui->mPadTable->setRowHeight(i,25);
    QTableWidgetItem *item;
    ui->mPadTable->setItem( i, 0, item = new QTableWidgetItem(iter.key()) );
    item->setData( ACTUAL_PIN, iter.key() );
    ui->mPadTable->setItem( i, 1, item = new QTableWidgetItem(iter.value().description()) );
    item->setFlags( Qt::ItemIsEnabled );
    if( iter.value().isEmpty() )
      item->setBackgroundColor( QColor(Qt::yellow).lighter() );
    i++;
    }

  ui->mPadTable->setCurrentCell( curRow, 0 );
  connect( ui->mPadTable, &QTableWidget::cellChanged, this, &SdDPads::cmCellEditComplete );
  }





//Append to association table full list of used in plate pins
void SdDPads::cmAccumUsedPins()
  {
  //Scan plate and accum used in part implementations used pads
  mPlate->forEachConst( dctPartImp, [this] (SdObject *obj) -> bool {
    SdGraphPartImp *imp = dynamic_cast<SdGraphPartImp*>(obj);
    if( imp != nullptr )
      imp->accumUsedPins( mMap );
    return true;
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
  ui->mPadTable->setRowHeight( row, 25 );
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
    QString key = item->data(ACTUAL_PIN).toString();
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
    SdPadAssociation assoc( mMap, name, SdProjectItem::getDefaultAuthor() );
    //Creating header
    SdLibraryHeader hdr;
    hdr.mName = name;
    hdr.mType = assoc.getType();
    hdr.mAuthor = SdProjectItem::getDefaultAuthor();
    hdr.mTime = SdTime2x::current();
    hdr.mClass = assoc.getClass();
    mUid = hdr.uid();
    //Store in library
    SdObjectFactory::insertObject( hdr, assoc.write() );
    QMessageBox::information( this, tr("Info"), tr("Pad association saved") );
    }
  }




//Assign associations from loaded table
void SdDPads::cmAssociationUse()
  {
  //Assign associations from loaded table
  SdPadAssociation *assoc = sdObjectOnly<SdPadAssociation>( SdDGetObject::getObject(dctPadAssociation, tr("Select pad association table"), this) );
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
  SdPadAssociation *assoc = sdObjectOnly<SdPadAssociation>( SdDGetObject::getObject(dctPadAssociation, tr("Select pad association table"), this) );
  if( assoc != nullptr ) {
    ui->mAssociationName->setText( assoc->getName() );
    mMap = assoc->getMap();
    //Update visual pin table
    updatePinTable();
    }
  }





//Complete edit pin-to-pad association
void SdDPads::cmCellEditComplete(int row, int column)
  {
  if( column == 0 ) {
    //Edit enable only for pin name
    QString newPin = ui->mPadTable->item( row, 0 )->text();
    QString oldPin = ui->mPadTable->item( row, 0 )->data( ACTUAL_PIN ).toString();
    if( newPin != oldPin ) {
      //Pin name changed
      if( !newPin.isEmpty() ) {
        //New name present
        if( mMap.contains(oldPin) )
          //For new pin name insert previous pad defined for this pin
          mMap.insert( newPin, mMap.value(oldPin) );
        else
          //No previous pin - insert default pad
          mMap.insert( newPin, SdPad() );
        }
      //If present, then remove previous pin
      if( mMap.contains(oldPin) )
        mMap.remove( oldPin );
      //Update visual table
      ui->mPadTable->item( row, 0 )->setData( ACTUAL_PIN, newPin );
      ui->mPadTable->item( row, 1 )->setText( mMap.value(newPin).description() );
      if( mMap.value(newPin).isEmpty() )
        ui->mPadTable->item( row, 1 )->setBackgroundColor( QColor(Qt::yellow).lighter() );
      }
    }
  }





//On clicked pad description cell show pad master
void SdDPads::cmCellClicked(int row, int column)
  {
  if( column ) {
    QString pin = ui->mPadTable->item(row,0)->text();
    if( mMap.contains(pin) ) {
      //Show pad master
      SdDPadMaster master( mMap.value(pin), this );
      if( master.exec() ) {
        //Update pad description
        mMap.insert( pin, master.pad() );
        ui->mPadTable->setItem( row, 1, new QTableWidgetItem( master.pad().description() ) );
        if( master.pad().isEmpty() )
          ui->mPadTable->item( row, 1 )->setBackgroundColor( QColor(Qt::yellow).lighter() );
        }
      }
    }
  }



