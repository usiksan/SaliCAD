#ifndef VRMLNODEAPPERANCE_H
#define VRMLNODEAPPERANCE_H

#include "VrmlNode.h"

class VrmlNodeApperance : public VrmlNode
  {
    VrmlNodePtr mMaterial;
    VrmlNodePtr mTexture;
    VrmlNodePtr mTextureTransform;
  public:
    VrmlNodeApperance();
  };

#endif // VRMLNODEAPPERANCE_H
