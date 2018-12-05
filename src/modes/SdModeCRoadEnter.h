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

#include <QList>

class SdModeCRoadEnter : public SdModeCommon
  {
    SdPoint         mSource;      //Source point of path
    SdPoint         mFirst;       //First point of segment
    SdPoint         mMiddle;      //Vertex point
    SdPoint         mBarMiddle;   //Vertex point with subject to barriers
    SdPoint         mLast;        //Point of cursor
    SdPoint         mBarLast;     //Point of cursor with subject to barriers
    SdPoint         mPrevMove;    //Previous entered point
    SdPropRoad      mProp;        //Current properties for road
    SdPropVia       mViaProp;     //Current properties for vias
    SdStratum       mStack;       //Available stratum stack at current point
    SdRuleBlock     mRule;        //Rule block for segment
    SdPoint         mTargetPoint; //Target point
    QList<SdPoint>  mSmartPath;   //Auto roaded path to smart point

    SdObjectPtrList mEnterPath;   //Created path

    bool            mShowNet;     //Show current net with enter color
    SdBarrierList   mPads;        //Barriers for pads on vias
    SdBarrierList   mRoads;       //Barriers for roads

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
    virtual void    reset() override;
    virtual void    drawStatic(SdContext *ctx) override;
    virtual void    drawDynamic(SdContext *ctx) override;
    virtual int     getPropBarId() const override;
    virtual void    propGetFromBar() override;
    virtual void    propSetToBar() override;
    virtual void    enterPoint(SdPoint p) override;
    virtual void    cancelPoint(SdPoint) override;
    virtual SdPoint enterPrev() override;
    virtual void    movePoint(SdPoint p) override;
    virtual QString getStepHelp() const override;
    virtual QString getModeThema() const override;
    virtual QString getStepThema() const override;
    virtual int     getCursor() const override;
    virtual int     getIndex() const override;
    virtual void    keyDown(int key, QChar ch) override;

  private:
    //Plate where road being inserted
    SdPItemPlate   *plate() { return dynamic_cast<SdPItemPlate*>(mObject); }
    void            getNetOnPoint( SdPoint p, SdStratum s, QString *netName, int *destStratum );
    void            calcFirstSmartPoint();
    void            calcNextSmartPoint(SdPoint fromPoint);
    SdPoint         checkRoad( SdPoint p1, SdPoint p2 ) const;
    bool            isBarriersContains( const SdBarrierList &bar, SdPoint p ) const;
    //Check if mSmartPath available
    bool            checkSmartPath() const;
    //Check if four vertex path available
    bool            checkSmartPath4( SdPoint p1, SdPoint p2, SdPoint p3, SdPoint p4 );

    //Try build smart path
    void            buildSmartPath( SdPoint p1, SdPoint p2 );

    //Append trace with store history
    void            addTrace( SdObject *obj, const QString msg );

    //Change trace layer
    void            changeTraceLayer();

    //Change to trace layer
    void            changeToTraceLayer( SdStratum dest );

    //Add via to change trace layer
    void            addVia( SdStratum newStratum );

    //First point enter
    void            firstPointEnter( bool enter );
  };

#endif // SDMODECROADENTER_H
