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
    VrmlNodeApperance( const VrmlNodeApperance *apperance );

    // VrmlNode interface
  public:
    virtual void      parse(SdScanerVrml *scaner) override;
    virtual VrmlNode *copy() const override { return new VrmlNodeApperance( this ); }
  };

#endif // VRMLNODEAPPERANCE_H
