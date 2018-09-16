/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Dialog for selection from list of string
*/
#include "SdDStringFromList.h"
#include "ui_SdDStringFromList.h"

#include <QListWidget>
#include <QTextBrowser>




SdDStringFromList::SdDStringFromList( const QStringList &lst, std::function<QString( QString key )> fun, QWidget *parent) :
  QDialog(parent),
  mGetDescription(fun),
  ui(new Ui::SdDStringFromList)
  {
  ui->setupUi(this);

  ui->mList->addItems( lst );
  //When current row changed we change description for this row
  connect( ui->mList, &QListWidget::currentRowChanged, this, [this] (int row ) {
    Q_UNUSED(row)
    QString str = ui->mList->currentItem()->text();
    ui->mDescription->setText( mGetDescription(str) );
    } );
  ui->mList->setCurrentRow(0);
  }




SdDStringFromList::~SdDStringFromList()
  {
  delete ui;
  }




//Result of selection
QString SdDStringFromList::result() const
  {
  return ui->mList->currentItem()->text();
  }
