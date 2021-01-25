#ifndef VRMLNODECOLOR_H
#define VRMLNODECOLOR_H

#include "VrmlColor.h"
#include "VrmlNode.h"

class VrmlNodeColor : public VrmlNode
  {
    VrmlColorList mColorList;
  public:
    VrmlNodeColor();
  };

#endif // VRMLNODECOLOR_H
