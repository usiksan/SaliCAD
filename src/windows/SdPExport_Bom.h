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
#include <QTableWidget>
#include <QCheckBox>
#include <QJsonArray>


class SdPExport_Bom : public QWizardPage
  {
    Q_OBJECT

    SdProjectItem *mProjectItem; //Project item from which made export
    QJsonArray     mList;        //Component list
  public:
    SdPExport_Bom( SdProjectItem *item, int step, SdPMasterList *list, QWidget *parent = nullptr);
  };

#endif // SDPEXPORT_BOM_H
