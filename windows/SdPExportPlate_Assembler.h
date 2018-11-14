/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Creation files for smt automat.
*/
#ifndef SDPEXPORTPLATE_ASSEMBLER_H
#define SDPEXPORTPLATE_ASSEMBLER_H

#include "SdPMasterList.h"
#include "objects/SdPItemPlate.h"

#include <QLineEdit>
#include <QTableWidget>

class SdWEditorGraphPlate;

class SdPExportPlate_Assembler : public QWizardPage
  {
    Q_OBJECT

  public:
    SdPExportPlate_Assembler( SdWEditorGraphPlate *editor, SdPItemPlate *plate, int step, SdPMasterList *list, QWidget *parent = nullptr );
  };

#endif // SDPEXPORTPLATE_ASSEMBLER_H
