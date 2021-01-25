#ifndef VRMLNODECOORDINATE_H
#define VRMLNODECOORDINATE_H

#include "VrmlNode.h"
#include "VrmlVector.h"

class VrmlNodeCoordinate : public VrmlNode
  {
    VrmlVectorList mPoint;
  public:
    VrmlNodeCoordinate();
  };

#endif // VRMLNODECOORDINATE_H
