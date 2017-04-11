/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/
#ifndef SDMODETZOOMWINDOW_H
#define SDMODETZOOMWINDOW_H

#include "SdModeTemp.h"

class SdModeTZoomWindow : public SdModeTemp
  {
    SdPoint mFirst;
    SdPoint mSecond;
    const int sFirst = 0, sSecond = 1;
  public:
    SdModeTZoomWindow( SdWEditorGraph *editor, SdProjectItem *obj );

    // SdMode interface
  public:
    virtual void    drawDynamic(SdContext *ctx) override;
    virtual void    enterPoint(SdPoint) override;
    virtual void    cancelPoint(SdPoint) override;
    virtual void    movePoint(SdPoint) override;
    virtual QString getStepHelp() const override;
    virtual QString getModeHelp() const override;
    virtual QString getStepThema() const override;
    virtual int     getCursor() const override;
    virtual int     getIndex() const override;
  };

#endif // SDMODETZOOMWINDOW_H
