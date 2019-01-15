/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Creation report files:
   - bom
   - specification
   - list
   - cvs
   - json
*/
#include "SdPExport_Bom.h"

SdPExport_Bom::SdPExport_Bom(SdProjectItem *item, int step, SdPMasterList *list, QWidget *parent) :
  QWizardPage( parent ),
  mProjectItem(item)
  {
  //Refistration of master in master list
  setMinimumWidth(800);
  list->addMaster( tr("Reports"), tr("Creates report files:\n- BOM\n- specification\n- list of components\n- cvs\n- json"), step, QString(":/pic/gerberExport.png") );

  }
