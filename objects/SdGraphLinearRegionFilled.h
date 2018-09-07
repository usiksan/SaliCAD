/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Graphical object Filled Region. It's region bounded lines and filled with color
*/
#ifndef SDGRAPHLINEARREGIONFILLED_H
#define SDGRAPHLINEARREGIONFILLED_H

#include "SdGraphLinearRegion.h"


#define SD_TYPE_REGION_FILLED "RegionFilled"


class SdGraphLinearRegionFilled : public SdGraphLinearRegion
  {
  public:
    SdGraphLinearRegionFilled();
    SdGraphLinearRegionFilled( const SdPointList list, const SdPropLine &propLine );

    // SdObject interface
  public:
    virtual QString getType() const override;

    // SdGraph interface
  public:
    virtual void draw(SdContext *dc) override;
  };

#endif // SDGRAPHLINEARREGIONFILLED_H
