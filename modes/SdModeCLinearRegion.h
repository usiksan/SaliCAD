/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Mode for closed regions
*/
#ifndef SDMODECLINEARREGION_H
#define SDMODECLINEARREGION_H

#include "SdModeCLinear.h"

class SdModeCLinearRegion : public SdModeCLinear
  {
    SdPointList    mList;       //Region vertex points
    SdPoint        mMiddle;     //Yet no entered vertex point
    SdPoint        mPrevMove;   //Previous entered point

    const int sFirstPoint = 0, sNextPoint = 1;
  public:
    SdModeCLinearRegion( SdWEditorGraph *editor, SdProjectItem *obj );

    // SdMode interface
  public:
    virtual void    drawDynamic(SdContext *ctx) override;
    virtual void    enterPoint(SdPoint enter) override;
    virtual void    cancelPoint(SdPoint) override;
    virtual void    movePoint(SdPoint p) override;
    virtual void    enterPrev() override;
    virtual QString getStepHelp() const override;
    virtual QString getModeHelp() const override;
    virtual QString getStepThema() const override;
    virtual int     getCursor() const override;
    virtual int     getIndex() const override;
  };

#endif // SDMODECLINEARREGION_H
