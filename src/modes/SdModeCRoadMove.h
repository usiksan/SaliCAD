/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Mode for move plate trace road

  When moving cursor we find nearest point to move from. This point is road-middle or road end.
  Road end available only if two or less roads vertex directly or through via.

  Finded point is source point. When press left button we select source point and drag.

  With move can be used:
  - 2 segments
  - 3 segments
  - 2 segments with via
  When road-middle point drag

*/
#ifndef SDMODECROADMOVE_H
#define SDMODECROADMOVE_H

#include "SdModeCommon.h"
#include "objects/SdPItemPlate.h"
#include "objects/SdBarrier.h"
#include "objects/SdPropRoad.h"
#include "objects/SdRuleBlock.h"
#include "objects/SdSelector.h"
#include "objects/SdGraphTracedRoad.h"

#include <QList>

class SdGraphTracedRoad;
class SdGraphTracedVia;

//flyNone,  //Point can not fly, it is fixed position
//flyAny,   //Point fly in any direction
//flyHorz,  //Point fly in horizontal direction
//flyVert,  //Point fly in vertical direction
//flyForw,  //Point fly with 45 degree in forward direction "/"
//flyBack   //Point fly with 45 degree in backward direction "\"

class SdModeCRoadMove : public SdModeCommon
  {
    SdPropRoad         mProp;         //Current properties for road moved segment
    SdPropRoad         mProp1;        //Current properties for road first segment
    SdPropRoad         mProp2;        //Current properties for road second segment
    SdPropVia          mViaProp;      //Current properties for vias

    SdRuleBlock        mRule;         //Rule block for net

    SdBarrierList      mPads;         //Barriers for pads on vias
    SdBarrierList      mRoads;        //Barriers for roads of moved segment
    SdBarrierList      mRoads1;       //Barriers for roads of first segment
    SdBarrierList      mRoads2;       //Barriers for roads of second segment

    SdPoint            mSourcePoint;  //Source point for move
    SdSnapMask         mSourceType;   //Current source point type
    SdGraph           *mSourceObject; //Object with source point

    SdGraphTracedRoad *mSegment,
                      *mSegment1,
                      *mSegment2;
    SdGraphTracedVia  *mVia;

    SdPoint            mSource1,      //Source point of segment. It not moved
                       mSource2,
                       mMove1,        //Moved point of segment 1
                       mMove2,        //Moved point of segment 2 or via position
                       mIntersect;    //Appeared intersect point of two segments segment1 and segment2

    int                mDirX1,        //Direction of moved points of segments
                       mDirY1,        // it may be 0 - not moved
                       mDirX2,        //           1 - direct relation
                       mDirY2;        //          -1 - inverse relation

    bool               mMoveSource1,  //Flag for source moving syncronous with mMove points
                       mMoveSource2;

    SdSelector         mFragment;
    SdPoint            mPrevMove;

    const int sFindRoad = 0, sMoveRoad = 1;
  public:
    SdModeCRoadMove( SdWEditorGraph *editor, SdProjectItem *obj );

    // SdMode interface
  public:
    virtual void reset() override;
    virtual void    drawStatic(SdContext *ctx) override;
    virtual void    drawDynamic(SdContext *ctx) override;
    virtual void    enterPoint(SdPoint) override;
    virtual void    movePoint(SdPoint p) override;
    virtual void    cancelPoint(SdPoint) override;
    virtual void    beginDrag(SdPoint p) override;
    virtual void    dragPoint(SdPoint p) override;
    virtual void    stopDrag(SdPoint p) override;
    virtual QString getStepHelp() const override;
    virtual QString getModeThema() const override;
    virtual QString getStepThema() const override;
    virtual int     getCursor() const override;
    virtual int     getIndex() const override;

  private:
    //Plate where road being inserted
    SdPItemPlate   *plate() { return dynamic_cast<SdPItemPlate*>(mObject); }

    //Change segment1 and segment2
    void            changeSegments();

    //Update segment. We create or delete segment if nessesery and change its position
    void            updateSegment(SdPropRoad &prop, SdGraphTracedRoadPtr &segment, SdPoint a, SdPoint b );
  };

#endif // SDMODECROADMOVE_H
