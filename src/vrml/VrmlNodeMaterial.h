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
    VrmlNodeMaterial( const VrmlNodeMaterial *material );

    // VrmlNode interface
  public:
    virtual VrmlNode *copy() const override { return new VrmlNodeMaterial( this ); }
    virtual bool      parse(SdScanerVrml *scaner, const QString &fieldType) override;
  };

#endif // VRMLNODEMATERIAL_H
