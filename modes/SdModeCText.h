/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Mode for text edit
*/
#ifndef SDMODECTEXT_H
#define SDMODECTEXT_H

#include "SdModeCTextual.h"

class SdModeCText : public SdModeCTextual
  {
  public:
    SdModeCText( SdWEditorGraph *editor, SdProjectItem *obj );

    // SdMode interface
  public:
    virtual void drawDynamic(SdContext *ctx) override;
    virtual void enterPoint(SdPoint) override;
    virtual void cancelPoint(SdPoint) override;
    virtual QString getStepHelp() const override;
    virtual QString getModeHelp() const override;
    virtual QString getStepThema() const override;
    virtual int getIndex() const override;

    // SdModeCTextual interface
  protected:
    virtual void cancelEdit() override;
    virtual void applyEdit() override;
  };

#endif // SDMODECTEXT_H
