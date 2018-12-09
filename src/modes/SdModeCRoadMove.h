/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Mode for move plate trace road
*/
#ifndef SDMODECROADMOVE_H
#define SDMODECROADMOVE_H

#include "SdModeCommon.h"
#include "objects/SdPItemPlate.h"
#include "objects/SdBarrier.h"
#include "objects/SdPropRoad.h"
#include "objects/SdRuleBlock.h"

#include <QList>

class SdGraphTracedRoad;
class SdGraphTracedVia;

class SdModeCRoadMove : public SdModeCommon
  {
    SdPropRoad      mProp;        //Current properties for road
    SdPropVia       mViaProp;     //Current properties for vias
    SdRuleBlock     mRule;        //Rule block for segment
    SdPoint         mTargetPoint; //Target point

    SdBarrierList   mPads;        //Barriers for pads on vias
    SdBarrierList   mRoads;       //Barriers for roads

//    SdGraphTracedRoad *mSegment1;
//    SdGraphTracedRoad *mSegment2;


    const int sFindRoad = 0, sMoveRoad = 1;
  public:
    SdModeCRoadMove( SdWEditorGraph *editor, SdProjectItem *obj );

    // SdMode interface
  public:
    virtual void reset() override;
    virtual void    drawStatic(SdContext *ctx) override;
    virtual void drawDynamic(SdContext *ctx) override;
    virtual void movePoint(SdPoint) override;
    virtual void    cancelPoint(SdPoint) override;
    virtual void beginDrag(SdPoint) override;
    virtual void dragPoint(SdPoint) override;
    virtual void stopDrag(SdPoint) override;
    virtual QString getStepHelp() const override;
    virtual QString getModeThema() const override;
    virtual QString getStepThema() const override;
    virtual int     getCursor() const override;
    virtual int     getIndex() const override;

  private:
    //Plate where road being inserted
    SdPItemPlate   *plate() { return dynamic_cast<SdPItemPlate*>(mObject); }
  };

#endif // SDMODECROADMOVE_H
