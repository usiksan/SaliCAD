/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Mode for line
*/
#ifndef SDMODECLINEARLINE_H
#define SDMODECLINEARLINE_H

#include "SdModeCLinear.h"

class SdModeCLinearLine : public SdModeCLinear
  {
    SdPoint        mFirst;      //First point
    SdPoint        mMiddle;     //Vertex point
    SdPoint        mPrevMove;   //Previous entered point

    SdPoint        mSmartPoint; //Точка разумного ввода
    SdSnapMask     mSmartType;  //Тип разумной точки

    static SdPoint mOffset;     //Предположительная очередная точка

    const int sFirstPoint = 0, sNextPoint = 1;
  public:
    SdModeCLinearLine( SdWEditorGraph *editor, SdProjectItem *obj );

    // SdMode interface
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
  };

#endif // SDMODECLINEARLINE_H
