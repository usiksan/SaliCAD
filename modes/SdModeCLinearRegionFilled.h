/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Mode for closed and filled regions
*/
#ifndef SDMODECLINEARREGIONFILLED_H
#define SDMODECLINEARREGIONFILLED_H

#include "SdModeCLinearRegion.h"

class SdModeCLinearRegionFilled : public SdModeCLinearRegion
  {
  public:
    SdModeCLinearRegionFilled( SdWEditorGraph *editor, SdProjectItem *obj );

    // SdMode interface
  public:
    virtual QString getStepHelp() const override;
    virtual QString getModeThema() const override;
    virtual QString getStepThema() const override;
    virtual int getIndex() const override;

    // SdModeCLinearRegion interface
  protected:
    virtual void addRegion() override;
  };

#endif // SDMODECLINEARREGIONFILLED_H
