/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Insertion fragment from library
*/
#ifndef SDMODECFRAGMENT_H
#define SDMODECFRAGMENT_H

#include "SdModeCommon.h"
#include "objects/SdProp.h"
#include "objects/SdSelector.h"
#include "objects/SdProject.h"


class SdModeCFragment : public SdModeCommon
  {
    SdSelector      mPaste;      //Врагмент для вставки (копирования)
    SdProject      *mPastePrj;   //Проект, из которого производится вставка
  public:
    SdModeCFragment( SdWEditorGraph *editor, SdProjectItem *obj );

    // SdMode interface
  public:
    virtual void activate() override;
    virtual void reset() override;
    virtual void drawStatic(SdContext *ctx) override;
    virtual void drawDynamic(SdContext *ctx) override;
    virtual int getPropBarId() const override;
    virtual void enterPoint(SdPoint) override;
    virtual void cancelPoint(SdPoint) override;
    virtual void movePoint(SdPoint) override;
    virtual QString getStepHelp() const override;
    virtual QString getModeThema() const override;
    virtual QString getStepThema() const override;
    virtual int getCursor() const override;
    virtual int getIndex() const override;
  };

#endif // SDMODECFRAGMENT_H
