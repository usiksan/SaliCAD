/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Mode for enter plate trace road
*/
#ifndef SDMODECROADENTER_H
#define SDMODECROADENTER_H

#include "SdModeCommon.h"
#include "objects/SdPItemPlate.h"
#include "objects/SdBarrier.h"
#include "objects/SdPropRoad.h"
#include "objects/SdRuleBlock.h"

class SdModeCRoadEnter : public SdModeCommon
  {
    //SdPointList    mPath;        //Created path
    SdPoint        mFirst;       //First point of segment
    SdPoint        mMiddle;      //Vertex point
    SdPoint        mBarMiddle;   //Vertex point with subject to barriers
    SdPoint        mLast;        //Point of cursor
    SdPoint        mBarLast;     //Point of cursor with subject to barriers
    //bool           mCatch;       //If true, then mLast point catched
    SdPoint        mPrevMove;    //Previous entered point
    SdPropRoad     mProp;        //Current properties for road
    SdPropVia      mViaProp;     //Current properties for vias
    SdStratum      mStack;       //Available stratum stack
    SdRuleBlock    mRule;        //Rule block for segment
    SdPoint        mSmartPoint;  //Smart point
    SdPoint        mSmartMiddle; //Smart middle point near middle, from which straight to smartPoint
    SdPointList    mSmartPath;   //Auto roaded path to smart point

    bool           mShowNet;     //Show current net with enter color
    SdBarrierList  mPads;
    SdBarrierList  mRoads;

    const int sFirstPoint = 0, sNextPoint = 1;

    enum {
      catchNone,
      catchFinish,
      catchOrthoX,
      catchOrthoY,
      catchDiagQuad1,
      catchDiagQuad2
      }            mCatch;       //Catch type

    void rebuildBarriers();
  public:
    SdModeCRoadEnter( SdWEditorGraph *editor, SdProjectItem *obj );

    // SdMode interface
  public:
    virtual void    drawStatic(SdContext *ctx) override;
    virtual void    drawDynamic(SdContext *ctx) override;
    virtual int     getPropBarId() const override;
    virtual void    propGetFromBar() override;
    virtual void    propSetToBar() override;
    virtual void enterPoint(SdPoint p) override;
    virtual void cancelPoint(SdPoint) override;
    virtual void    movePoint(SdPoint p) override;
    virtual QString getStepHelp() const override;
    virtual QString getModeThema() const override;
    virtual QString getStepThema() const override;
    virtual int     getCursor() const override;
    virtual int     getIndex() const override;

  private:
    SdPItemPlate   *plate() { return dynamic_cast<SdPItemPlate*>(mObject); }
    void            getNetOnPoint( SdPoint p, SdStratum s, QString *netName, int *destStratum );
    void            calcFirstSmartPoint();
    void            calcNextSmartPoint();
    SdPoint         checkRoad( SdPoint p1, SdPoint p2 ) const;
    bool            isBarriersContains( const SdBarrierList &bar, SdPoint p ) const;
  };

#endif // SDMODECROADENTER_H
