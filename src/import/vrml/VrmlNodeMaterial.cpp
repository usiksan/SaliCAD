#include "VrmlNodeMaterial.h"
#include "SdScanerVrml.h"

VrmlNodeMaterial::VrmlNodeMaterial() :
  VrmlNode(),
  mAmbientIntensity(0.2),
  mDiffuseColor( 0.8, 0.8, 0.8 ),
  mEmissiveColor(),
  mShininnes(0.2),
  mSpecularColor(),
  mTransparency(0.0)
  {

  }


VrmlNodeMaterial::VrmlNodeMaterial(const VrmlNodeMaterial *material) :
  VrmlNode( material ),
  mAmbientIntensity( material->mAmbientIntensity ),
  mDiffuseColor( material->mDiffuseColor ),
  mEmissiveColor( material->mEmissiveColor ),
  mShininnes( material->mShininnes ),
  mSpecularColor( material->mSpecularColor ),
  mTransparency( material->mTransparency )
  {

  }



void VrmlNodeMaterial::set(VrmlColor ambient, VrmlColor diffuse, VrmlColor emissive, VrmlColor specular, float shininnes, float transparency)
  {
  mDiffuseColor = diffuse;
  if( diffuse.mRed != 0.0 )
    mAmbientIntensity = ambient.mRed / diffuse.mRed;
  else if( diffuse.mGreen != 0.0 )
    mAmbientIntensity = ambient.mGreen / diffuse.mGreen;
  else if( diffuse.mBlue != 0.0 )
    mAmbientIntensity = ambient.mBlue / diffuse.mBlue;
  else
    mAmbientIntensity = 0.0;

  mEmissiveColor = emissive;
  mSpecularColor = specular;
  mShininnes     = shininnes;
  mTransparency  = transparency;
  }




Sd3dMaterial VrmlNodeMaterial::to3dMaterial() const
  {
  Sd3dMaterial mat;
  mat.mAmbient   = mDiffuseColor.toColor( mAmbientIntensity );
  mat.mDiffuse   = mDiffuseColor.toColor();
  mat.mEmissive  = mEmissiveColor.toColor();
  mat.mSpecular  = mSpecularColor.toColor();
  mat.mShininnes = mShininnes;
  return mat;
  }






bool VrmlNodeMaterial::parse(SdScanerVrml *scaner, const QString &fieldType)
  {
  if( fieldType == QStringLiteral("ambientIntensity") )
    scaner->tokenNeedValueFloat( 'f', mAmbientIntensity, QStringLiteral("No ambient instensity") );

  else if( fieldType == QStringLiteral("diffuseColor") )
    mDiffuseColor.parse( scaner );

  else if( fieldType == QStringLiteral("emissiveColor") )
    mEmissiveColor.parse( scaner );

  else if( fieldType == QStringLiteral("shininess") )
    scaner->tokenNeedValueFloat( 'f', mShininnes, QStringLiteral("No shininnes") );

  else if( fieldType == QStringLiteral("specularColor") )
    mSpecularColor.parse( scaner );

  else if( fieldType == QStringLiteral("transparency") )
    scaner->tokenNeedValueFloat( 'f', mTransparency, QStringLiteral("No transparency") );

  else return false;

  return true;
  }
