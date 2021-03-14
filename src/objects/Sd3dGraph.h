/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Base object for 3d graphics
*/
#ifndef SD3DGRAPH_H
#define SD3DGRAPH_H

#include "SdGraph.h"


class Sd3dGraph : public SdGraph
  {
  public:
    Sd3dGraph();

    // SdObject interface
  public:
    virtual SdClass getClass() const override;
  };

#endif // SD3DGRAPH_H
