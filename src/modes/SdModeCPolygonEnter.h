/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Mode for enter plate trace polygon
*/
#ifndef SDMODECPOLYGONENTER_H
#define SDMODECPOLYGONENTER_H

#include "SdModeCommon.h"
#include "objects/SdPItemPlate.h"
#include "objects/SdBarrier.h"
#include "objects/SdPropRoad.h"
#include "objects/SdRuleBlock.h"


class SdModeCPolygonEnter : public SdModeCommon
  {
    SdPointList    mList;       //Region vertex points
    SdPoint        mMiddle;     //Yet no entered vertex point
    SdPoint        mPrevMove;   //Previous entered point
    SdPropPolygon  mProp;       //Current properties for polygon
    SdPoint        mSmart;      //Smart point with which we retrieve net name for polygon

    const int sFirstPoint = 0, sNextPoint = 1;
  public:
    SdModeCPolygonEnter( SdWEditorGraph *editor, SdProjectItem *obj );

    // SdMode interface
  public:
    virtual void    drawStatic(SdContext *ctx) override;
    virtual void    drawDynamic(SdContext *ctx) override;
    virtual int     getPropBarId() const override;
    virtual void    propGetFromBar() override;
    virtual void    propSetToBar() override;
    virtual void    enterPoint(SdPoint enter) override;
    virtual void    cancelPoint(SdPoint) override;
    virtual void    movePoint(SdPoint p) override;
    virtual SdPoint enterPrev() override;
    virtual QString getStepHelp() const override;
    virtual QString getModeThema() const override;
    virtual QString getStepThema() const override;
    virtual int     getCursor() const override;
    virtual int     getIndex() const override;
    virtual void    keyDown( int key, QChar ch ) override;

  private:
    SdPItemPlate *plate() { return dynamic_cast<SdPItemPlate*>(mObject); }
  };

#endif // SDMODECPOLYGONENTER_H
