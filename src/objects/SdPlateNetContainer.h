/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Plate net container. Contains net segments with appropriate traced objects.
  Used for segments accumulation.
*/
#ifndef SDPLATENETCONTAINER_H
#define SDPLATENETCONTAINER_H

#include "SdPoint.h"
#include "SdStratum.h"
#include "SdGraphTraced.h"

class SdPlateNetContainer
  {
  public:
    SdPlateNetContainer();
    virtual ~SdPlateNetContainer();

    virtual void addNetSegment( SdGraphTraced *traced, const QString netName, SdStratum s, SdPoint p1, SdPoint p2 ) = 0;
  };

#endif // SDPLATENETCONTAINER_H
