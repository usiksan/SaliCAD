/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Mode for part pin
*/
#ifndef SDMODECPARTPIN_H
#define SDMODECPARTPIN_H

#include "SdModeCTextual.h"


class SdModeCPartPin : public SdModeCTextual
  {
    SdPoint        mOrigin;      //Pin place point
    QString        mNumber;      //Pin number
    SdPoint        mNumberPos;   //Number position point
    SdPoint        mNamePos;     //Name position point
    SdPoint        mSmartPoint;  //Smart mode point
    int            mSmartType;   //Smart mode point type

    static SdPoint
      mSmartNumber,              //Offset from previous number
      mSmartName;                //Offset from previous name
    static QString mPrevNumber;  //Previous number
  public:
    SdModeCPartPin(SdWEditorGraph *editor, SdProjectItem *obj);

    // SdMode interface
  public:
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

    // SdModeCTextual interface
  protected:
    virtual void cancelEdit() override;
    virtual void applyEdit() override;
  };

#endif // SDMODECPARTPIN_H
