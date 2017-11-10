/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/
#include "SdDRegistation.h"
#include "ui_SdDRegistation.h"

SdDRegistation::SdDRegistation(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::SdDRegistation)
  {
  ui->setupUi(this);
  }

SdDRegistation::~SdDRegistation()
  {
  delete ui;
  }
