/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/
#include "SdDNetUnion.h"
#include "ui_SdDNetUnion.h"
#include "modes/SdModeCNetWire.h"
#include <QMessageBox>

SdDNetUnion::SdDNetUnion(const QString first, const QString second, QWidget *parent) :
  QDialog(parent),
  ui(new Ui::SdDNetUnion)
  {
  ui->setupUi(this);

  ui->mFirst->setText( tr("Assign '%1'").arg(first) );
  ui->mSecond->setText( tr("Assign '%2'").arg(second) );
  }

SdDNetUnion::~SdDNetUnion()
  {
  delete ui;
  }


void SdDNetUnion::accept()
  {
  if( ui->mFirst->isChecked() ) done( SdModeCNetWire::renFirst );
  else if( ui->mSecond->isChecked() ) done( SdModeCNetWire::renSecond );
  else QMessageBox::warning( this, tr("Error"), tr("You must select one variant or cancel") );
  }

void SdDNetUnion::reject()
  {
  done( SdModeCNetWire::renCancel );
  }
