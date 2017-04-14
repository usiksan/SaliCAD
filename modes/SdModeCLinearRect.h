/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Mode for rectangle
*/
#ifndef SDMODECLINEARRECT_H
#define SDMODECLINEARRECT_H

#include "SdModeCLinear.h"

class SdModeCLinearRect : public SdModeCLinear
  {
  protected:
    SdPoint        mFirst;      //Точка первого угла
    SdPoint        mPrevMove;   //Текущая точка

    SdPoint        mSmartPoint; //Точка разумного ввода
    quint64        mSmartType;  //Тип разумной точки

    static SdPoint mOffset; //Предположительная очередная точка

    const int sFirstCorner = 0, sSecondCorner = 1;
  public:
    SdModeCLinearRect( SdWEditorGraph *editor, SdProjectItem *obj );

    // SdMode interface
    virtual void    drawDynamic(SdContext *ctx) override;
    virtual void    enterPoint(SdPoint) override;
    virtual void    cancelPoint(SdPoint) override;
    virtual void    movePoint(SdPoint) override;
    virtual void    enterPrev() override;
    virtual QString getStepHelp() const override;
    virtual QString getModeHelp() const override;
    virtual QString getStepThema() const override;
    virtual int     getCursor() const override;
    virtual int     getIndex() const override;
  protected:
    //Insert rectangle into graph object
    virtual void    addRect();
  };

#endif // SDMODECLINEARRECT_H
