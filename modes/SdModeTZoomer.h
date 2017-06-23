/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Temporary mode for zoom-in and zoom-out picture
*/
#ifndef SDMODETZOOMER_H
#define SDMODETZOOMER_H

#include "SdModeTemp.h"
#include "objects/SdScaler.h"

class SdModeTZoomer : public SdModeTemp
  {
    bool      mZoomIn;
  public:
    SdModeTZoomer( bool zoomIn, SdWEditorGraph *editor, SdProjectItem *obj );

    // SdMode interface
    virtual void    enterPoint(SdPoint point) override;
    virtual void    cancelPoint(SdPoint point) override;
    virtual QString getStepHelp() const override;
    virtual QString getModeThema() const override;
    virtual QString getStepThema() const override;
    virtual int     getCursor() const override;
    virtual int     getIndex() const override;
  };

#endif // SDMODETZOOMER_H
