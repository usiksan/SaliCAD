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
#ifndef SDPEXPORT_BOM_H
#define SDPEXPORT_BOM_H

#include "SdPMasterList.h"
#include "objects/SdPItemPlate.h"

#include <QLineEdit>
#include <QCheckBox>
#include <QJsonObject>
#include <QListWidget>
#include <QPushButton>



class SdPExport_Bom : public QWizardPage
  {
    Q_OBJECT

    SdProjectItem *mProjectItem;      //Project item from which made export

    QCheckBox     *mCurrentItem;      //If checked info collected only from current item, otherwise - from all project
    QCheckBox     *mGenBom;           //If checked then generate BOM or specification
    QListWidget   *mBomPatternList;   //List of patterns for BOM
    QPushButton   *mBom;              //Button to generate BOM
    QCheckBox     *mGenRegistry;      //If checked then generate component registry
    QCheckBox     *mGenRegistryGroup; //If checked then components in registry grouped if consistently the same
    QListWidget   *mRegistryPattern;  //List of patterns for registry
    QPushButton   *mRegistry;         //Button to generate regirstry
  public:
    SdPExport_Bom( SdProjectItem *item, int step, SdPMasterList *list, QWidget *parent = nullptr);

  public slots:
    //Generate BOM
    void genBom();

    //Generate Registry
    void genRegistry();

    // QWizardPage interface
  public:
    virtual bool validatePage() override;
    virtual int  nextId() const override { return -1; }

  private:
    QList<QJsonObject> accumulate();
  };

#endif // SDPEXPORT_BOM_H
