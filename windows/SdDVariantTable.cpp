#include "SdDVariantTable.h"
#include "ui_SdDVariantTable.h"

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
    connect( ui->mDefFields, &QTableWidget::cellChanged, this, &SdDVariantTable::defChanged );
    }
  else {
    ui->mVariantAppendField->setEnabled(false);
    ui->mVariantFieldDelete->setEnabled(false);
    ui->mRowInsert->setEnabled(false);
    ui->mRowDelete->setEnabled(false);
    }
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
      int rows = ui->mVariantTable->rowCount();
      for( int i = rows * mVariantFieldCount + column; i >= 0; i -= mVariantFieldCount )
        mVariantTable.removeAt(i);
      ui->mVariantTable->removeColumn( column );
      mVariantFieldCount--;
      }
    }
  else
    QMessageBox::warning( this, tr("Warning!"), tr("No selected field for deletion. Select partial field and try again.") );
  }







//Insert row to variant table
void SdDVariantTable::rowInsert()
  {
  //Source row for copy. And insertion will be after this place
  int row = ui->mVariantTable->currentRow();
  if( row < 0 )
    row = ui->mVariantTable->rowCount() - 1;
  //Insert row
  int dest = (row + 2) * mVariantFieldCount;
  if( row >= 0 ) {
    //Insertion with copy
    int src = (row + 1) * mVariantFieldCount;
    for( int i = 0; i < mVariantFieldCount; i++ )
      mVariantTable.insert( dest + i, mVariantTable.at(src + i) );
    }
  else
    //Insertion fields with default values
    for( int i = 0; i < mVariantFieldCount; i++ )
      mVariantTable.insert( dest + i, mDefFields.value( mVariantTable.at(i) ) );
  //Visual row appendion
  ui->mVariantTable->insertRow(row+1);
  ui->mVariantTable->setRowHeight(row+1,20);
  for( int i = 0; i < mVariantFieldCount; i++ )
    ui->mVariantTable->setItem( row+1, i, new QTableWidgetItem( mVariantTable.at(dest+i) )   );
  }





//Delete row from variant table
void SdDVariantTable::rowDelete()
  {
  int row = ui->mVariantTable->currentRow();
  if( row >= 0 ) {
    //Calculate begin for deleting
    int start = mVariantFieldCount + mVariantFieldCount * row;
    for( int i = 0; i < mVariantFieldCount; i++ )
      mVariantTable.removeAt(start);
    //Remove visual row
    ui->mVariantTable->removeRow(row);
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
  if( mVariantFieldCount ) {
    int rows = mVariantTable.count() / mVariantFieldCount;
    for( int r = rows; r > 1; r-- )
      mVariantTable.insert( r * mVariantFieldCount, QString() );
    mVariantTable.insert( mVariantFieldCount, name );
    }
  else {
    //First field appended
    mVariantTable.clear();
    mVariantTable.append( name );
    }
  mVariantFieldCount++;
  buildVariantTable();
  }




//Check if field present in variant table
bool SdDVariantTable::isVariantField(const QString field)
  {
  for( int i = 0; i < mVariantFieldCount; i++ )
    if( mVariantTable.at(i) == field ) return true;
  return false;
  }
