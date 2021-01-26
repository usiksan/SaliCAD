#ifndef VRMLNODEAPPEARANCE_H
#define VRMLNODEAPPEARANCE_H

#include "VrmlNode.h"

class VrmlNodeAppearance : public VrmlNode
  {
    VrmlNodePtr mMaterial;
    VrmlNodePtr mTexture;
    VrmlNodePtr mTextureTransform;
  public:
    VrmlNodeAppearance();
    VrmlNodeAppearance( const VrmlNodeAppearance *apperance );
    virtual ~VrmlNodeAppearance();

    // VrmlNode interface
  public:
    virtual void      parse(SdScanerVrml *scaner) override;
    virtual VrmlNode *copy() const override { return new VrmlNodeAppearance( this ); }
  };

#endif // VRMLNODEAPPEARANCE_H
