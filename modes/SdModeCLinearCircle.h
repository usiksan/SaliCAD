/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Mode for draw arc's
*/
#ifndef SDMODECLINEARCIRCLE_H
#define SDMODECLINEARCIRCLE_H

#include "SdModeCLinear.h"

class SdModeCLinearCircle : public SdModeCLinear
  {
  protected:
    SdPoint    mCenter;          //Circle center [Цент окружности]
    int        mRadius;          //Circle radius [Радиус окружности]

    SdPoint    mSmartPoint;      //Smart mode point [Точка разумного ввода]
    SdSnapMask mSmartType;       //Smart point type [Тип разумной точки]

    static int mPreviousRadius;  //Previous circle radius [Радиус предыдущей окружности]
    enum { sCenter, sRadius, sLast };
  public:
    SdModeCLinearCircle( SdWEditorGraph *editor, SdProjectItem *obj );

    // SdMode interface
  public:
    virtual void    drawDynamic(SdContext *ctx) override;
    virtual void    enterPoint(SdPoint p) override;
    virtual void    cancelPoint(SdPoint) override;
    virtual void    movePoint(SdPoint) override;
    virtual SdPoint enterPrev() override;
    virtual QString getStepHelp() const override;
    virtual QString getModeThema() const override;
    virtual QString getStepThema() const override;
    virtual int getCursor() const override;
    virtual int getIndex() const override;
  };

#endif // SDMODECLINEARCIRCLE_H
