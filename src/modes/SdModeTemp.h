/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Base mode for temporary modes.
*/
#ifndef SDMODETEMP_H
#define SDMODETEMP_H

#include "SdMode.h"

class SdModeTemp : public SdMode
  {
    SdMode *mMainMode; //Main mode temporary replaced this mode
  public:
    SdModeTemp( SdWEditorGraph *editor, SdProjectItem *obj );

    //Set new main mode
    void    setMainMode( SdMode *mode ) { mMainMode = mode; }

    //Get main mode
    SdMode *mainMode() { return mMainMode; }

    // SdMode interface
  public:
    virtual void drawStatic(SdContext *ctx) override;
  };

#endif // SDMODETEMP_H
