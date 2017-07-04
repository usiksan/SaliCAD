/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/
#include "SdDEnterPosition.h"
#include "ui_SdDEnterPosition.h"

static bool prevRef;
static int  prevX,prevY,prevZ;

SdDEnterPosition::SdDEnterPosition(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::SdDEnterPosition)
  {
  ui->setupUi(this);

  ui->m
  }



SdDEnterPosition::~SdDEnterPosition()
  {
  delete ui;
  }

void SdDEnterPosition::changeEvent(QEvent *e)
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
