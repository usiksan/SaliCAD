/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Dialog for selection result name when union two nets.
*/
#include "SdDNetUnion.h"
#include "ui_SdDNetUnion.h"
#include "modes/SdModeCNetWire.h"
#include "SdDHelp.h"
#include <QMessageBox>
#include <QPushButton>

SdDNetUnion::SdDNetUnion(const QString first, const QString second, QWidget *parent) :
  QDialog(parent),
  ui(new Ui::SdDNetUnion)
  {
  ui->setupUi(this);

  ui->mFirst->setText( tr("Assign '%1'").arg(first) );
  ui->mSecond->setText( tr("Assign '%2'").arg(second) );

  connect( ui->buttonBox->button(QDialogButtonBox::Help), &QPushButton::clicked, this, [this] () {
    SdDHelp::help( QString("SdDNetUnion.htm"), this );
    } );
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
