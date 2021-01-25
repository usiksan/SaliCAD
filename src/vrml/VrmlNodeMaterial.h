#ifndef VRMLNODEMATERIAL_H
#define VRMLNODEMATERIAL_H

#include "VrmlTypes.h"
#include "VrmlColor.h"
#include "VrmlNode.h"

class VrmlNodeMaterial : public VrmlNode
  {
    VrmlFloat mAmbientIntensity;
    VrmlColor mDiffuseColor;
    VrmlColor mEmissiveColor;
    VrmlFloat mShininnes;
    VrmlColor mSpecularColor;
    VrmlFloat mTransparency;
  public:
    VrmlNodeMaterial();
  };

#endif // VRMLNODEMATERIAL_H
