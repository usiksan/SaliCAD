/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Creation dxf file from current object
*/
#include "SdPExport_Dxf.h"

SdPExport_Dxf::SdPExport_Dxf(SdProjectItem *item, int step, SdPMasterList *list, QWidget *parent) :
  QWizardPage( parent ),
  mProjectItem(item)
  {
  //Refistration of master in master list
  setMinimumWidth(800);
  list->addMaster( tr("DXF file"), tr("Creates DXF file from current object"), step, QString(":/pic/gerberExport.png") );

  }
