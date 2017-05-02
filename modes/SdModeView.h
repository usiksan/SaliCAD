/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Mode for view graph object.
  All other mode except this, if set - enter edit mode of graph object.
  This mode NOT enter edit mode, so it may be used in simple view of object.
*/
#ifndef SDMODEVIEW_H
#define SDMODEVIEW_H

#include "SdMode.h"

class SdModeView : public SdMode
  {
    SdPoint mBeginDrag; //Point at drag begins
  public:
    SdModeView(SdWEditorGraph *editor, SdProjectItem *obj);

    // SdMode interface
  public:
    virtual void    drawStatic(SdContext *ctx) override;
    virtual void    beginDrag(SdPoint p) override;
    virtual void    dragPoint(SdPoint p) override;
    virtual void    stopDrag( SdPoint ) override;
    virtual QString getStepHelp() const override;
    virtual QString getModeHelp() const override;
    virtual QString getStepThema() const override;
    virtual int     getCursor() const override;
    virtual int     getIndex() const override;
  };

#endif // SDMODEVIEW_H
