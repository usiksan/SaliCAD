/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Separate pcb area of schematic sheet
*/
#ifndef SDGRAPHAREA_H
#define SDGRAPHAREA_H

#include "SdGraph.h"
#include "SdPointList.h"

#define SD_TYPE_PART_PIN "PartPin"


class SdGraphArea : public SdGraph
  {
  public:
    SdGraphArea();
  };

#endif // SDGRAPHAREA_H
