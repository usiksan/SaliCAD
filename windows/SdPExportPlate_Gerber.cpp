/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Creation gerber file on plate
*/
#include "SdPExportPlate_Gerber.h"

SdPExportPlate_Gerber::SdPExportPlate_Gerber(SdPItemPlate *plate, int step, SdPMasterList *list, QWidget *parent) :
  QWizardPage( parent ),
  mPlate(plate)
  {
  list->addMaster( tr("Gerber"), tr("Creates gerber files for plate"), step, QString("") );

  }
