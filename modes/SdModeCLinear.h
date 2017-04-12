/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Base mode for insert linear objects
*/
#ifndef SDMODECLINEAR_H
#define SDMODECLINEAR_H

#include "SdModeCommon.h"
#include "objects/SdPropLine.h"

class SdModeCLinear : public SdModeCommon
  {
  protected:
    SdPropLine mPropLine; //Props for linear objects
  public:
    SdModeCLinear( SdWEditorGraph *editor, SdProjectItem *obj );

    // SdMode interface
    virtual int  getPropBarId() const override;
    virtual void propGetFromBar() override;
    virtual void propSetToBar() override;
  };

#endif // SDMODECLINEAR_H
