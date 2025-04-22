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
#include <QSpinBox>
#include <QCheckBox>

class SdWEditorGraphPlate;

class SdPExportPlate_Gerber : public QWizardPage
  {
    Q_OBJECT

    SdWEditorGraphPlate *mEditor;    //Graphic editor widget

    SdPItemPlate        *mPlate;     //Plate which we export to gerber
    QLineEdit           *mFile;      //File name for single generation

    QLineEdit           *mGroupPath; //Path to store group fiels
    QTableWidget        *mGroup;     //Table for multiple gerber generation

    QCheckBox           *mGridEna;   //!< Enable grid generation
    QSpinBox            *mColumns;   //!< Column count in grid
    QDoubleSpinBox      *mColumnGap; //!< Gap beatween columns
    QSpinBox            *mRows;      //!< Row count in grid
    QDoubleSpinBox      *mRowGap;    //!< Gap beatween rows

    bool                 mGenerated; //True if at least one time generation was pressed
  public:
    SdPExportPlate_Gerber( SdWEditorGraphPlate *editor, SdPItemPlate *plate, int step, SdPMasterList *list, QWidget *parent = nullptr );

  public slots:
    //On press file select button. We show file select dialog.
    void onFileSelect();

    //On press button "generate". We generate Gerber to file with current layers map
    void onGenerate();

    //On press button "Layers...". We opern layers dialog
    void onLayers();

    //On press button "group generate". We generate Gerber to goupt path for each selected pattern
    void onGroupGenerate();

    //On press file group select button. We show directory select dialog
    void onGroupPathSelect();

    //On cell clicked
    void onCellClicked( int row, int column );

  private:
    //Generate gerber for current view to file
    void generation(const QString fileName );

    // QWizardPage interface
  public:
    virtual int  nextId() const override { return -1; }
    virtual bool validatePage() override;
  };

#endif // SDPEXPORTPLATE_GERBER_H
