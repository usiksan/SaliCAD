#ifndef VRMLNODEAPPEARANCE_H
#define VRMLNODEAPPEARANCE_H

#include "VrmlNode.h"
#include "VrmlNodeMaterial.h"

class VrmlNodeAppearance : public VrmlNode
  {
    VrmlNodePtr mMaterial;
    VrmlNodePtr mTexture;
    VrmlNodePtr mTextureTransform;
  public:
    VrmlNodeAppearance();
    VrmlNodeAppearance( const VrmlNodeAppearance *apperance );
    virtual ~VrmlNodeAppearance();

    VrmlNodeMaterial *material() const { return dynamic_cast<VrmlNodeMaterial*>(mMaterial); }
    VrmlColor         color() const;
    // VrmlNode interface
  public:
    virtual VrmlNode *copy() const override { return new VrmlNodeAppearance( this ); }
    virtual bool      parse(SdScanerVrml *scaner, const QString &fieldType) override;
  };

#endif // VRMLNODEAPPEARANCE_H
