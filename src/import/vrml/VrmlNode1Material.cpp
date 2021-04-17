#include "VrmlNode1Material.h"
#include "SdScanerVrml.h"

VrmlNode1Material::VrmlNode1Material() :
  VrmlNode1()
  {

  }



VrmlNodeMaterial VrmlNode1Material::material(int index) const
  {
  VrmlNodeMaterial m;
  if( index >= 0 && index < mAmbientColor.count() && index < mDiffuseColor.count() &&
      index < mEmissiveColor.count() && index < mSpecularColor.count() &&
      index < mShinisses.count() && index < mTransparency.count() )
    m.set( mAmbientColor.at(index), mDiffuseColor.at(index), mEmissiveColor.at(index), mSpecularColor.at(index), mShinisses.at(index), mTransparency.at(index) );
  return m;
  }



bool VrmlNode1Material::parse(SdScanerVrml *scaner, const QString &fieldType)
  {
  if( fieldType == QStringLiteral("ambientColor") )
    scaner->parseColorTable( mAmbientColor, QStringLiteral("Need ambient color table") );

  else if( fieldType == QStringLiteral("diffuseColor") )
    scaner->parseColorTable( mDiffuseColor, QStringLiteral("Need diffuse color table") );

  else if( fieldType == QStringLiteral("emissiveColor") )
    scaner->parseColorTable( mEmissiveColor, QStringLiteral("Need emissive color table") );

  else if( fieldType == QStringLiteral("specularColor") )
    scaner->parseColorTable( mSpecularColor, QStringLiteral("Need specular color table") );

  else if( fieldType == QStringLiteral("shininess") )
    scaner->parseFloatTable( mShinisses, QStringLiteral("Need shininess table") );

  else if( fieldType == QStringLiteral("transparency") )
    scaner->parseFloatTable( mTransparency, QStringLiteral("Need transparency table") );

  else return false;

  return true;
  }

