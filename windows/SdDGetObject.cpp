/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/

#include "SdDGetObject.h"
#include "ui_SdDGetObject.h"

SdDGetObject::SdDGetObject(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::SdDGetObject)
  {
  ui->setupUi(this);
  }

SdDGetObject::~SdDGetObject()
  {
  delete ui;
  }

void SdDGetObject::changeEvent(QEvent *e)
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
