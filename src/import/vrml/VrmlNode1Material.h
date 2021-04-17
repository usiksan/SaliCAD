#ifndef VRMLNODE1MATERIAL_H
#define VRMLNODE1MATERIAL_H

#include "VrmlNode1.h"
#include "VrmlColor.h"
#include "VrmlNodeMaterial.h"

class VrmlNode1Material : public VrmlNode1
  {
    VrmlColorList mAmbientColor;
    VrmlColorList mDiffuseColor;
    VrmlColorList mEmissiveColor;
    VrmlColorList mSpecularColor;
    QList<float>  mShinisses;
    QList<float>  mTransparency;
  public:
    VrmlNode1Material();

    bool      isAmbientValid( int index ) const { return index >= 0 && index < mAmbientColor.count(); }
    bool      isDiffuseValid( int index ) const { return index >= 0 && index < mDiffuseColor.count(); }
    bool      isEmissiveValid( int index ) const { return index >= 0 && index < mEmissiveColor.count(); }
    bool      isSpecularValid( int index ) const { return index >= 0 && index < mSpecularColor.count(); }
    bool      isShinissesValid( int index ) const { return index >= 0 && index < mShinisses.count(); }
    bool      isTransparencyValid( int index ) const { return index >= 0 && index < mTransparency.count(); }

    VrmlColor ambientColor( int index ) const { return mAmbientColor.at(index); }
    VrmlColor diffuseColor( int index ) const { return mDiffuseColor.at(index); }
    VrmlColor emissiveColor( int index ) const { return mEmissiveColor.at(index); }
    VrmlColor specularColor( int index ) const { return mSpecularColor.at(index); }
    float     shinisses( int index ) const { return mShinisses.at(index); }
    float     transparency( int index ) const { return mTransparency.at(index); }

    VrmlNodeMaterial material( int index ) const;

    // VrmlNode interface
  public:
    virtual bool parse(SdScanerVrml *scaner, const QString &fieldType) override;
  };

#endif // VRMLNODE1MATERIAL_H
