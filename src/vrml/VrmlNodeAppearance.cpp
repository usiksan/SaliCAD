#include "VrmlNodeAppearance.h"
#include "VrmlNodeMaterial.h"
#include "SdScanerVrml.h"

VrmlNodeAppearance::VrmlNodeAppearance() :
  VrmlNode(),
  mMaterial(nullptr),
  mTexture(nullptr),
  mTextureTransform(nullptr)
  {

  }



VrmlNodeAppearance::VrmlNodeAppearance(const VrmlNodeAppearance *apperance) :
  VrmlNode( apperance ),
  mMaterial(nullptr),
  mTexture(nullptr),
  mTextureTransform(nullptr)
  {
  mMaterial         = makeCopy( apperance->mMaterial );
  mTexture          = makeCopy( apperance->mTexture );
  mTextureTransform = makeCopy( apperance->mTextureTransform );
  }



VrmlNodeAppearance::~VrmlNodeAppearance()
  {
  deleteNode( mMaterial );
  deleteNode( mTexture );
  deleteNode( mTextureTransform );
  }




VrmlColor VrmlNodeAppearance::color() const
  {
  VrmlNodeMaterial *material = dynamic_cast<VrmlNodeMaterial*>(mMaterial);
  if( material == nullptr )
    return VrmlColor(-1.0);
  return material->color();
  }







bool VrmlNodeAppearance::parse(SdScanerVrml *scaner, const QString &fieldType)
  {
  if( fieldType == QStringLiteral("material") )
    mMaterial = parse2Declaration( scaner );
  else if( fieldType == QStringLiteral("texture") )
    mTexture = parse2Declaration( scaner );
  else if( fieldType == QStringLiteral("textureTransform") )
    mTextureTransform = parse2Declaration( scaner );
  else return false;
  return true;
  }
