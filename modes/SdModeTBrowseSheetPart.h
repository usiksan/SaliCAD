/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/
#ifndef SDMODETBROWSESHEETPART_H
#define SDMODETBROWSESHEETPART_H

#include "SdModeTemp.h"
#include "objects/SdPItemPlate.h"
#include "objects/SdGraphSymImp.h"


class SdModeTBrowseSheetPart : public SdModeTemp
  {
    SdGraphSymImp *mSymImp; //Symbol implement behind cursor
    SdProjectItem *mPlate;  //Plate object which part place mode
  public:
    SdModeTBrowseSheetPart( SdWEditorGraph *editor, SdProjectItem *obj, SdProjectItem *plate );

    // SdMode interface
  public:
    virtual SdPoint enterPrev() override;
    virtual QString getStepHelp() const override;
    virtual QString getModeThema() const override;
    virtual QString getStepThema() const override;
    virtual int getCursor() const override;
    virtual int getIndex() const override;
  };

#endif // SDMODETBROWSESHEETPART_H
