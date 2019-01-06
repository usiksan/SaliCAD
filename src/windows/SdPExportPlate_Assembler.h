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
#include <QCheckBox>

class SdWEditorGraphPlate;

class SdPExportPlate_Assembler : public QWizardPage
  {
    Q_OBJECT

    SdWEditorGraphPlate *mEditor;    //Graphic editor widget

    SdPItemPlate        *mPlate;     //Plate which we export to smt

    QCheckBox           *mBottom;    //If checked then used bottom side with mirrored
    QCheckBox           *mSolder;    //Perform solder paste dispence
    QLineEdit           *mDelimiter; //Delimiter for export files
  public:
    SdPExportPlate_Assembler( SdWEditorGraphPlate *editor, SdPItemPlate *plate, int step, SdPMasterList *list, QWidget *parent = nullptr );
  };

#endif // SDPEXPORTPLATE_ASSEMBLER_H
