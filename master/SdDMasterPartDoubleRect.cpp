/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/
#include "SdDMasterPartDoubleRect.h"
#include "ui_SdDMasterPartDoubleRect.h"

SdDMasterPartDoubleRect::SdDMasterPartDoubleRect(SdProjectItem *item, QWidget *parent) :
  SdDMasterPart( item, parent),
  ui(new Ui::SdDMasterPartDoubleRect)
  {
  ui->setupUi(this);
  }

SdDMasterPartDoubleRect::~SdDMasterPartDoubleRect()
  {
  delete ui;
  }
