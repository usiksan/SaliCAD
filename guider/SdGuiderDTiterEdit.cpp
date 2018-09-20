#include "SdGuiderDTiterEdit.h"
#include "ui_SdGuiderDTiterEdit.h"

SdGuiderDTiterEdit::SdGuiderDTiterEdit( SdGuiderTiter *titer, QWidget *parent) :
  QDialog(parent),
  mTiter(titer),
  ui(new Ui::SdGuiderDTiterEdit)
  {
  ui->setupUi(this);

  //Fill initial table
  ui->mTable->setColumnCount(2);
  ui->mTable->setRowCount(mTiter->mContens.count());
  ui->mTable->setHorizontalHeaderLabels( {tr("Language"), tr("Titer contens")} );
  ui->mTable->setColumnWidth(1,400);
  int row = 0;
  for( auto iter = mTiter->mContens.cbegin(); iter != mTiter->mContens.cend(); iter++ ) {
    ui->mTable->setRowHeight( row, 25 );
    ui->mTable->setItem( row, 0, new QTableWidgetItem(iter.key()) );
    ui->mTable->setItem( row, 1, new QTableWidgetItem(iter.value()) );
    }

  connect( ui->mTable, &QTableWidget::cellChanged, this, &SdGuiderDTiterEdit::onChanged );
  connect( ui->mAppend, &QPushButton::clicked, this, &SdGuiderDTiterEdit::onAppend );
  connect( ui->mDelete, &QPushButton::clicked, this, &SdGuiderDTiterEdit::onDelete );
  }




SdGuiderDTiterEdit::~SdGuiderDTiterEdit()
  {
  delete ui;
  }




void SdGuiderDTiterEdit::onChanged(int row, int column)
  {
  Q_UNUSED(column)
  //disconnect( ui->mTable, &QTableWidget::cellChanged, this, &SdGuiderDTiterEdit::onChanged );
  QString key = ui->mTable->item(row,0)->text();
  QString value = ui->mTable->item(row,1)->text();
  if( !key.isEmpty() && !value.isEmpty() ) {
    mTiter->mContens.insert( key, value );
    }
  }




void SdGuiderDTiterEdit::onAppend()
  {
  disconnect( ui->mTable, &QTableWidget::cellChanged, this, &SdGuiderDTiterEdit::onChanged );
  ui->mTable->insertRow(0);
  ui->mTable->setRowHeight( 0, 25 );
  ui->mTable->setItem( 0, 0, new QTableWidgetItem() );
  ui->mTable->setItem( 0, 1, new QTableWidgetItem() );
  connect( ui->mTable, &QTableWidget::cellChanged, this, &SdGuiderDTiterEdit::onChanged );
  }




void SdGuiderDTiterEdit::onDelete()
  {
  disconnect( ui->mTable, &QTableWidget::cellChanged, this, &SdGuiderDTiterEdit::onChanged );
  int row = ui->mTable->currentRow();
  if( row >= 0 && row < ui->mTable->rowCount() ) {
    QString key = ui->mTable->item(row,0)->text();
    mTiter->mContens.remove( key );
    ui->mTable->removeRow( row );
    }
  connect( ui->mTable, &QTableWidget::cellChanged, this, &SdGuiderDTiterEdit::onChanged );
  }




void SdGuiderDTiterEdit::changeEvent(QEvent *e)
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
