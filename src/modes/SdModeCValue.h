/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Mode for change position and properties of value
*/
#ifndef SDMODECVALUE_H
#define SDMODECVALUE_H

#include "SdModeCTextual.h"

class SdGraphValue;

class SdModeCValue : public SdModeCTextual
  {
    SdGraphValue *mValue; //Edited value
    int           mIndex; //Mode index
  public:
    SdModeCValue( SdWEditorGraph *editor, SdProjectItem *obj, SdPropText *prp, int index );

    // SdMode interface
  public:
    virtual void    activate() override;
    virtual void    drawStatic(SdContext *ctx) override;
    virtual void    drawDynamic(SdContext *ctx) override;
    virtual int     getPropBarId() const override;
    virtual void    propGetFromBar() override;
    virtual void    propSetToBar() override;
    virtual void    enterPoint(SdPoint) override;
    virtual void    cancelPoint(SdPoint) override;
    virtual void    movePoint(SdPoint p) override;
    virtual QString getStepHelp() const override;
    virtual QString getModeThema() const override;
    virtual QString getStepThema() const override;
    virtual int     getCursor() const override;
    virtual int     getIndex() const override;
  };

#endif // SDMODECVALUE_H
