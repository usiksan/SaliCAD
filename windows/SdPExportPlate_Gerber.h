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
#ifndef SDPEXPORTPLATE_GERBER_H
#define SDPEXPORTPLATE_GERBER_H

#include "SdPMasterList.h"
#include "objects/SdPItemPlate.h"

class SdPExportPlate_Gerber : public QWizardPage
  {
    Q_OBJECT

    SdPItemPlate *mPlate;
  public:
    SdPExportPlate_Gerber( SdPItemPlate *plate, int step, SdPMasterList *list, QWidget *parent = nullptr );
  };

#endif // SDPEXPORTPLATE_GERBER_H
