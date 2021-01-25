#ifndef VRMLNODECOMPOUND_H
#define VRMLNODECOMPOUND_H

#include "VrmlNode.h"

class VrmlNodeCompound : public VrmlNode
  {
    VrmlNodePtrList mChildren;
  public:
    VrmlNodeCompound();
  };

#endif // VRMLNODECOMPOUND_H
