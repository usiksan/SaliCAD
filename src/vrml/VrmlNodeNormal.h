#ifndef VRMLNODENORMAL_H
#define VRMLNODENORMAL_H

#include "VrmlNode.h"
#include "VrmlVector.h"

class VrmlNodeNormal : public VrmlNode
  {
    VrmlVectorList mVector;
  public:
    VrmlNodeNormal();
  };

#endif // VRMLNODENORMAL_H
