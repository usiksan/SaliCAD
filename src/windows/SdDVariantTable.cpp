/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  View and edit variant param table
*/
#include "SdDVariantTable.h"
#include "ui_SdDVariantTable.h"
#include "SdDHelp.h"

#include <QMessageBox>
#include <QInputDialog>

SdDVariantTable::SdDVariantTable(SdPItemVariant *var, bool editEna, QWidget *parent) :
  QDialog(parent),
  mVariant(var),
  ui(new Ui::SdDVariantTable)
  {
  ui->setupUi(this);

  mDefFields = mVariant->paramTable();
  mVariant->variantTableGet( mVariantFieldCount, mVariantTable );
  buildDefTable();
  buildVariantTable();

  if( editEna ) {
    connect( ui->mVariantAppendField, &QPushButton::clicked, this, &SdDVariantTable::variantFieldAppend );
    connect( ui->mVariantFieldDelete, &QPushButton::clicked, this, &SdDVariantTable::variantFieldDelete );
    connect( ui->mRowInsert, &QPushButton::clicked, this, &SdDVariantTable::rowInsert );
    connect( ui->mRowDelete, &QPushButton::clicked, this, &SdDVariantTable::rowDelete );
    connect( ui->mRowDeleteAll, &QPushButton::clicked, this, &SdDVariantTable::rowDeleteAll );
    connect( ui->mDefFields, &QTableWidget::cellChanged, this, &SdDVariantTable::defChanged );
    }
  else {
    mVariant = nullptr;
    ui->mVariantAppendField->setEnabled(false);
    ui->mVariantFieldDelete->setEnabled(false);
    ui->mRowInsert->setEnabled(false);
    ui->mRowDelete->setEnabled(false);
    }

  connect( ui->buttonBox->button(QDialogButtonBox::Help), &QPushButton::clicked, this, [this] () {
    SdDHelp::help( QString("SdDVariantTable.htm"), this );
    } );
  }




SdDVariantTable::~SdDVariantTable()
  {
  delete ui;
  }




//Append field to variant table
void SdDVariantTable::variantFieldAppend()
  {
  QString name = QInputDialog::getText( this, tr("Append field"), tr("Enter field name for append") );
  if( !name.isEmpty() )
    variantFieldAppendInt( name );
  }





//Delete field from variant table
void SdDVariantTable::variantFieldDelete()
  {
  int column = ui->mVariantTable->currentColumn();
  if( column >= 0 ) {
    if( QMessageBox::question( this, tr("Warning!"), tr("Are You sure to delete field '%1'? This operation is not undoable").arg(mVariantTable.at(column))) == QMessageBox::Yes ) {
      ui->mVariantTable->removeColumn( column );
      }
    }
  else
    QMessageBox::warning( this, tr("Warning!"), tr("No selected field for deletion. Select partial field and try again.") );
  }







//Insert row to variant table
void SdDVariantTable::rowInsert()
  {
  //Source row for copy. And insertion will be after this place
  int srcRow = ui->mVariantTable->currentRow();
  if( srcRow < 0 )
    srcRow = ui->mVariantTable->rowCount() - 1;
  //Visual row appendion
  int dstRow = srcRow + 1;
  ui->mVariantTable->insertRow(dstRow);
  ui->mVariantTable->setRowHeight(dstRow,20);
  int column = ui->mVariantTable->columnCount();
  if( srcRow >= 0 ) {
    //Insert with copy
    for( int i = 0; i < column; i++ )
      ui->mVariantTable->setItem( dstRow, i, new QTableWidgetItem( ui->mVariantTable->item(srcRow,i)->text() )   );
    }
  else {
    //Insert with default
    for( int i = 0; i < column; i++ )
      ui->mVariantTable->setItem( dstRow, i, new QTableWidgetItem( mDefFields.value(ui->mVariantTable->horizontalHeaderItem(i)->text()) )   );
    }
  }





//Delete row from variant table
void SdDVariantTable::rowDelete()
  {
  int row = ui->mVariantTable->currentRow();
  if( row >= 0 ) {
    //Remove visual row
    ui->mVariantTable->removeRow(row);
    }
  }





//Delete all rows from variant table
void SdDVariantTable::rowDeleteAll()
  {
  //Show query
  if( QMessageBox::question( this, tr("Warning!"), tr("Are You sure to delete ALL rows? This is non undoing operation. Are You sure?") ) == QMessageBox::Yes ) {
    while( ui->mVariantTable->rowCount() )
      ui->mVariantTable->removeRow(0);
    }
  }




//When edited def parameter
void SdDVariantTable::defChanged(int row, int column)
  {
  if( column == 0 ) {
    if( ui->mDefFields->item( row, column )->checkState() == Qt::Checked )
      //Append field to variant table
      variantFieldAppendInt( ui->mDefFields->item( row, 1)->text() );
    if( ui->mDefFields->item( row, column )->checkState() == Qt::Unchecked ) {
      //Remove field from variant table
      QString name = ui->mDefFields->item( row, 1 )->text();
      //Show query
      if( QMessageBox::question( this, tr("Warning!"), tr("You attempting delete field '%1' from variant table. All field column contens will be loss. This is non undoing operation. Are You sure?").arg(name)) == QMessageBox::Yes ) {
        //Find column
        int column;
        for( column = 0; column < ui->mVariantTable->columnCount(); column++ )
          if( ui->mVariantTable->horizontalHeaderItem(column)->text() == name )
            break;
        if( column < ui->mVariantTable->columnCount() )
          ui->mVariantTable->removeColumn( column );
        }
      }
    }
  }




void SdDVariantTable::changeEvent(QEvent *e)
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




//Build visual default param table
void SdDVariantTable::buildDefTable()
  {
  //Fill table
  //QStringList lst = SdDParamEditor::defParamList();
  ui->mDefFields->setColumnCount( 3 );
  ui->mDefFields->setRowCount( mDefFields.count() );
  //Table header
  ui->mDefFields->setHorizontalHeaderLabels( {tr("Has variant"), tr("Param name"), tr("Param value")} );
  //Table column width
  ui->mDefFields->setColumnWidth( 0, 70 );
  ui->mDefFields->setColumnWidth( 1, 120 );
  ui->mDefFields->setColumnWidth( 2, 150 );
  int row = 0;
  for( auto iter = mDefFields.cbegin(); iter != mDefFields.cend(); iter++ ) {
    ui->mDefFields->setRowHeight( row, 25 );
    QTableWidgetItem *item = new QTableWidgetItem();
    item->setFlags( Qt::ItemIsEnabled | Qt::ItemIsUserCheckable );
    item->setCheckState( isVariantField(iter.key()) ? Qt::Checked : Qt::Unchecked );
    ui->mDefFields->setItem( row, 0, item );
    ui->mDefFields->setItem( row, 1, item = new QTableWidgetItem( iter.key() ) );
    item->setFlags( Qt::ItemIsEnabled );
    ui->mDefFields->setItem( row, 2, item = new QTableWidgetItem( iter.value() ) );
    item->setFlags( Qt::ItemIsEnabled );
    row++;
    //item->setToolTip(  );
    }
  }




//Build visual variant table
void SdDVariantTable::buildVariantTable()
  {
  ui->mVariantTable->clear();
  if( mVariantFieldCount ) {
    //Setup column count
    ui->mVariantTable->setColumnCount( mVariantFieldCount );
    //Setup column width and header
    for( int i = 0; i < mVariantFieldCount; i++ ) {
      ui->mVariantTable->setColumnWidth( i, 100 );
      ui->mVariantTable->setHorizontalHeaderItem(i, new QTableWidgetItem( mVariantTable.at(i) ) );
      }


    //Setup row count
    int rows = mVariantTable.count() / mVariantFieldCount - 1;
    ui->mVariantTable->setRowCount( rows );

    //Fill individual cell
    int index = mVariantFieldCount;
    for( int r = 0; r < rows; r++ ) {
      ui->mVariantTable->setRowHeight( r, 20 );
      for( int c = 0; c < mVariantFieldCount; c++ ) {
        ui->mVariantTable->setItem( r, c, new QTableWidgetItem(mVariantTable.at(index++)) );
        }
      }
    }
  }




//Append field at position pos with name
void SdDVariantTable::variantFieldAppendInt(const QString name)
  {
  int pos = ui->mVariantTable->columnCount();
  ui->mVariantTable->insertColumn( pos );
  ui->mVariantTable->setColumnWidth( pos, 100 );
  ui->mVariantTable->setHorizontalHeaderItem( pos, new QTableWidgetItem(name) );
  int rows = ui->mVariantTable->rowCount();
  for( int i = 0; i < rows; i++ )
    ui->mVariantTable->setItem( i, pos, new QTableWidgetItem() );
  }




//Check if field present in variant table
bool SdDVariantTable::isVariantField(const QString field)
  {
  for( int i = 0; i < mVariantFieldCount; i++ )
    if( mVariantTable.at(i) == field ) return true;
  return false;
  }







void SdDVariantTable::accept()
  {
  if( mVariant ) {
    //Setup field count
    mVariantFieldCount = ui->mVariantTable->columnCount();
    mVariantTable.clear();
    //Scan fields name
    for( int i = 0; i < mVariantFieldCount; i++ )
      mVariantTable.append( ui->mVariantTable->horizontalHeaderItem(i)->text() );
    //Scan cell
    int row = ui->mVariantTable->rowCount();
    for( int r = 0; r < row; r++ )
      for( int c = 0; c < mVariantFieldCount; c++ )
        mVariantTable.append( ui->mVariantTable->item(r,c)->text() );
    mVariant->variantTableSet( mVariantFieldCount, mVariantTable, mVariant->getUndo() );
    }
  QDialog::accept();
  }
