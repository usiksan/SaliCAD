/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/
#include "SdDMasterPartDoubleSideLR.h"
#include "ui_SdDMasterPartDoubleSideLR.h"

SdDMasterPartDoubleSideLR::SdDMasterPartDoubleSideLR(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::SdDMasterPartDoubleSideLR)
  {
  ui->setupUi(this);
  }

SdDMasterPartDoubleSideLR::~SdDMasterPartDoubleSideLR()
  {
  delete ui;
  }
