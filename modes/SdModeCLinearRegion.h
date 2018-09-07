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
  protected:
    SdPointList    mList;       //Region vertex points
    SdPoint        mMiddle;     //Yet no entered vertex point
    SdPoint        mPrevMove;   //Previous entered point

    const int sFirstPoint = 0, sNextPoint = 1;
  public:
    SdModeCLinearRegion( SdWEditorGraph *editor, SdProjectItem *obj );

    // SdMode interface
  public:
    //Draw dinamic part of mode scene
    virtual void    drawDynamic(SdContext *ctx) override;
    virtual void    enterPoint(SdPoint enter) override;
    virtual void    cancelPoint(SdPoint) override;
    virtual void    movePoint(SdPoint p) override;
    virtual SdPoint enterPrev() override;
    virtual QString getStepHelp() const override;
    virtual QString getModeThema() const override;
    virtual QString getStepThema() const override;
    virtual int     getCursor() const override;
    virtual int     getIndex() const override;
    virtual void    keyDown(int key, QChar ch) override;

  protected:
    //Append region to edit object
    virtual void    addRegion();
  };

#endif // SDMODECLINEARREGION_H
