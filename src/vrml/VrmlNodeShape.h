#ifndef VRMLNODESHAPE_H
#define VRMLNODESHAPE_H

#include "VrmlNode.h"

class VrmlNodeShape : public VrmlNode
  {
    VrmlNodePtr mApperance;
    VrmlNodePtr mGeometry;
  public:
    VrmlNodeShape();
  };

#endif // VRMLNODESHAPE_H
