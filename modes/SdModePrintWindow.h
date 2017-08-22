/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  With this mode user fix print window
*/
#ifndef SDMODEPRINTWINDOW_H
#define SDMODEPRINTWINDOW_H

#include "SdModeTemp.h"
#include "objects/SdRect.h"

class SdModePrintWindow : public SdModeTemp
  {
    SdPoint mFirst;
    SdPoint mPrevMove;
    enum { sFirstCorner, sSecondCorner };
  public:
    SdModePrintWindow( SdWEditorGraph *editor, SdProjectItem *obj );

    // SdMode interface
  public:
    virtual void    drawDynamic(SdContext *ctx) override;
    virtual void    enterPoint(SdPoint enter) override;
    virtual void    cancelPoint(SdPoint) override;
    virtual void    movePoint(SdPoint p) override;
    virtual QString getStepHelp() const override;
    virtual QString getModeThema() const override;
    virtual QString getStepThema() const override;
    virtual int     getCursor() const override;
    virtual int     getIndex() const override;
  };

#endif // SDMODEPRINTWINDOW_H
