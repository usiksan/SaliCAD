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

    float ambientIntensity() const { return mAmbientIntensity; }
    float diffuseColor( int ic ) const { return mDiffuseColor.color(ic); }
    float emissiveColor( int ic ) const { return mEmissiveColor.color(ic); }
    float shininnes() const { return mShininnes; }
    float specularColor( int ic ) const { return mSpecularColor.color(ic); }
    float transparency() const { return mTransparency; }

    void  set( VrmlColor ambient, VrmlColor diffuse, VrmlColor emissive, VrmlColor specular, float shininnes, float transparency );

    // VrmlNode interface
  public:
    virtual VrmlNode *copy() const override { return new VrmlNodeMaterial( this ); }
    virtual bool      parse(SdScanerVrml *scaner, const QString &fieldType) override;
  };

#endif // VRMLNODEMATERIAL_H
