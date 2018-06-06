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
#ifndef SDMODECLINEARARC_H
#define SDMODECLINEARARC_H

#include "SdModeCLinear.h"


class SdModeCLinearArc : public SdModeCLinear
  {
    SdPoint    mCenter;      //Arc center
    SdPoint    mStart;       //Arc start point

    SdPoint    mSmartPoint;  //Smart mode point [Точка разумного ввода]
    SdSnapMask mSmartType;   //Smart point type [Тип разумной точки]

    static SdPoint mStartOffset, mStopOffset;
    enum { sCenter, sStart, sStop, sLast };
  public:
    SdModeCLinearArc( SdWEditorGraph *editor, SdProjectItem *obj );

    // SdMode interface
  public:
    virtual void    drawDynamic(SdContext *ctx) override;
    virtual void    enterPoint(SdPoint enter) override;
    virtual void    cancelPoint(SdPoint) override;
    virtual void    movePoint(SdPoint p) override;
    virtual void    enterPrev() override;
    virtual QString getStepHelp() const override;
    virtual QString getModeThema() const override;
    virtual QString getStepThema() const override;
    virtual int     getCursor() const override;
    virtual int     getIndex() const override;
  };

#endif // SDMODECLINEARARC_H
