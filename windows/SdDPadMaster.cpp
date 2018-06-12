/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/
#include "SdDPadMaster.h"
#include "ui_SdDPadMaster.h"

SdDPadMaster::SdDPadMaster(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::SdDPadMaster)
  {
  ui->setupUi(this);
  }

SdDPadMaster::~SdDPadMaster()
  {
  delete ui;
  }
