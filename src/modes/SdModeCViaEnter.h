/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Mode for enter via
*/
#ifndef SDMODECVIAENTER_H
#define SDMODECVIAENTER_H

#include "SdModeCommon.h"
#include "objects/SdPItemPlate.h"
#include "objects/SdBarrier.h"
#include "objects/SdPropRoad.h"
#include "objects/SdRuleBlock.h"

class SdModeCViaEnter : public SdModeCommon
  {
    SdPropVia       mViaProp;     //Current properties for vias
    SdBarrierList   mPads;        //Barriers for pads on vias
  public:
    SdModeCViaEnter(SdWEditorGraph *editor, SdProjectItem *obj);

    // SdMode interface
  public:
    virtual void    drawStatic(SdContext *ctx) override;
    virtual int     getPropBarId() const override;
    virtual void    propGetFromBar() override;
    virtual void    propSetToBar() override;
    virtual void    enterPoint(SdPoint) override;
    virtual void    cancelPoint(SdPoint) override;
    virtual QString getStepHelp() const override;
    virtual QString getModeThema() const override;
    virtual QString getStepThema() const override;
    virtual int     getCursor() const override;
    virtual int     getIndex() const override;

  private:
    //Plate where road being inserted
    SdPItemPlate   *plate() { return dynamic_cast<SdPItemPlate*>(mObject); }
  };

#endif // SDMODECVIAENTER_H
