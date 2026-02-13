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
#include <QList>

class SdWEditorGraphPlate;




struct SdGerberFile
  {
    QString     mFileName;  //!< Gerber file name
    QStringList mLayerList; //!< List of layers used for file generation
  };





using SdGerberFileList = QList<SdGerberFile>;

class SdPExportPlate_Gerber : public QWizardPage
  {
    Q_OBJECT

    SdWEditorGraphPlate *mEditor;         //!< Graphic editor widget

    //Single file generation
    SdPItemPlate        *mPlate;          //!< Plate which we export to gerber
    QLineEdit           *mFile;           //!< File name for single generation

    //Group file generation
    QLineEdit           *mGroupPath;      //!< Path to store group fiels
    QTableWidget        *mGroup;          //!< Table for multiple gerber generation
    SdGerberFileList     mGerberFileList; //!< List of gerber files available for this pcb

    //PCB multiplate grid
    QCheckBox           *mGridEna;        //!< Enable grid generation
    QSpinBox            *mColumns;        //!< Column count in grid
    QDoubleSpinBox      *mColumnGap;      //!< Gap beatween columns
    QSpinBox            *mRows;           //!< Row count in grid
    QDoubleSpinBox      *mRowGap;         //!< Gap beatween rows

    bool                 mGenerated;      //!< True if at least one time generation was pressed
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

    void addGerberName( const QString &gerber, const QString &base ) { addGerberName( gerber, {base}, QString("") ); }
    void addGerberName( const QString &gerber, const QString &base, const QString &lid1, bool addPcbContour = true ) { addGerberNames( gerber, {base}, lid1, addPcbContour ); }
    void addGerberNames( const QString &gerber, const QStringList &base, const QString &lid1, bool addPcbContour = true );
    // QWizardPage interface
  public:
    virtual int  nextId() const override { return -1; }
    virtual bool validatePage() override;
  };

#endif // SDPEXPORTPLATE_GERBER_H
