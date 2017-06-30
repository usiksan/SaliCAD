/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/
#include "SdDGetBus.h"
#include "ui_SdDGetBus.h"
#include <QStringList>

static QStringList previousBusList;

SdDGetBus::SdDGetBus(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::SdDGetBus)
  {
  ui->setupUi(this);
  }

SdDGetBus::~SdDGetBus()
  {
  delete ui;
  }

QStringList SdDGetBus::busList() const
  {

  }


void SdDGetBus::accept()
  {

  }
