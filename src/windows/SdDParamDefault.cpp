/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Dialog for default param selection and insertion.
  All parameters listed int table with description.
  User can check one or more parameters for insertion.
*/
#include "SdDParamDefault.h"
#include "ui_SdDParamDefault.h"
#include "SdDParamEditor.h"
#include "SdDHelp.h"
#include <QCheckBox>

SdDParamDefault::SdDParamDefault(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::SdDParamDefault)
  {
  ui->setupUi(this);

  //Fill table
  QStringList lst = SdDParamEditor::defParamList();
  ui->mTable->setColumnCount( 3 );
  ui->mTable->setRowCount( lst.count() );
  //Table header
  ui->mTable->setHorizontalHeaderLabels( {tr("Insert"), tr("Param name"), tr("Param description")} );
  //Table column width
  ui->mTable->setColumnWidth( 0, 70 );
  ui->mTable->setColumnWidth( 1, 120 );
  ui->mTable->setColumnWidth( 2, 500 );
  for( int row = 0; row < lst.count(); row++ ) {
    ui->mTable->setRowHeight( row, 25 );
    ui->mTable->setCellWidget( row, 0, new QCheckBox() );
    QTableWidgetItem *item;
    ui->mTable->setItem( row, 1, item = new QTableWidgetItem( lst.at(row) ) );
    item->setFlags( Qt::ItemIsEnabled );
    ui->mTable->setItem( row, 2, item = new QTableWidgetItem( SdDParamEditor::defParamDescription(lst.at(row))) );
    item->setFlags( Qt::ItemIsEnabled );
    item->setToolTip(  SdDParamEditor::defParamDescription(lst.at(row)) );
    }

  connect( ui->buttonBox->button(QDialogButtonBox::Help), &QPushButton::clicked, this, [this] () {
    SdDHelp::help( QString("SdDParamDefault.htm"), this );
    } );
  }




SdDParamDefault::~SdDParamDefault()
  {
  delete ui;
  }






//Return list of default params for insertion
QStringList SdDParamDefault::defParamList() const
  {
  QStringList res;
  int count = ui->mTable->rowCount();
  for( int row = 0; row < count; row++ )
    if( dynamic_cast<QCheckBox*>(ui->mTable->cellWidget( row, 0 ))->isChecked() )
      res.append( ui->mTable->item( row, 1 )->text() );
  return res;
  }




void SdDParamDefault::changeEvent(QEvent *e)
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
