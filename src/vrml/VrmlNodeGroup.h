#ifndef VRMLNODEGROUP_H
#define VRMLNODEGROUP_H

#include "VrmlVector.h"
#include "VrmlNodeCompound.h"

class VrmlNodeGroup : public VrmlNodeCompound
  {
    VrmlVector mBoxCenter;
    VrmlVector mBoxSize;
  public:
    VrmlNodeGroup();
  };

#endif // VRMLNODEGROUP_H
