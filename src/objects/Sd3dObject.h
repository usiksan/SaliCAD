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
#ifndef SD3DOBJECT_H
#define SD3DOBJECT_H

#include "SdGraph.h"


class Sd3dObject : public SdGraph
  {
  public:
    Sd3dObject();

    // SdObject interface
  public:
    virtual SdClass getClass() const override;
  };

#endif // SD3DOBJECT_H
