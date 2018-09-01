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

#include <QLineEdit>
#include <QTableWidget>

class SdWEditorGraphPlate;

class SdPExportPlate_Gerber : public QWizardPage
  {
    Q_OBJECT

    SdWEditorGraphPlate *mEditor; //Graphic editor widget
    SdPItemPlate        *mPlate;  //Plate which we export to gerber
    QLineEdit           *mFile;   //File name for single generation
    QTableWidget        *mGroup;  //Table for multiple gerber generation
  public:
    SdPExportPlate_Gerber( SdWEditorGraphPlate *editor, SdPItemPlate *plate, int step, SdPMasterList *list, QWidget *parent = nullptr );

  public slots:
    //On press file select button. We show file select dialog.
    void onFileSelect();

    //On press button "generate". We generate Gerber to file with current layers map
    void onGenerate();

    //On press button "Layers...". We opern layers dialog
    void onLayers();
  };

#endif // SDPEXPORTPLATE_GERBER_H
