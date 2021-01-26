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


void VrmlNodeMaterial::parse(SdScanerVrml *scaner)
  {
  if( !scaner->tokenNeed( '{', QStringLiteral("No material") ) )
    return;

  while( !scaner->matchToken('}') ) {
    if( scaner->isEndOfScan() ) {
      scaner->error( QStringLiteral("Uncompleted material") );
      return;
      }
    if( scaner->isError() )
      return;
    QString fieldType;
    if( !scaner->tokenNeedValue( 'n', fieldType, QStringLiteral("Need material field") ) )
      return;
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
    else {
      scaner->error( QStringLiteral("Undefined Material field %1").arg(fieldType) );
      return;
      }
    }
  }


