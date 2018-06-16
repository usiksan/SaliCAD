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

class SdModeCRoadEnter : public SdModeCommon
  {
    SdPoint        mFirst;      //First point
    SdPoint        mMiddle;     //Vertex point
    SdPoint        mPrevMove;   //Previous entered point
    SdPropRoad     mProp;       //Current properties for road
//    QString        mNetName;    //Net name for entered road
//    SdStratum      mActive;     //Current stratum

    bool           mShowNet;    //Show current net with enter color
    SdBarrierList  mPads;
    SdBarrierList  mRoads;

    const int sFirstPoint = 0, sNextPoint = 1;

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
    virtual void enterPoint(SdPoint) override;
    virtual void cancelPoint(SdPoint) override;
    virtual void movePoint(SdPoint) override;
    virtual QString getStepHelp() const override;
    virtual QString getModeThema() const override;
    virtual QString getStepThema() const override;
    virtual int getCursor() const override;
    virtual int getIndex() const override;
  };

#endif // SDMODECROADENTER_H
