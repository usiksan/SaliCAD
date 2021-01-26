#include "VrmlNodeApperance.h"
#include "SdScanerVrml.h"

VrmlNodeApperance::VrmlNodeApperance() :
  VrmlNode(),
  mMaterial(nullptr),
  mTexture(nullptr),
  mTextureTransform(nullptr)
  {

  }

VrmlNodeApperance::VrmlNodeApperance(const VrmlNodeApperance *apperance) :
  VrmlNode( apperance ),
  mMaterial(nullptr),
  mTexture(nullptr),
  mTextureTransform(nullptr)
  {
  mMaterial         = makeCopy( apperance->mMaterial );
  mTexture          = makeCopy( apperance->mTexture );
  mTextureTransform = makeCopy( apperance->mTextureTransform );
  }


void VrmlNodeApperance::parse(SdScanerVrml *scaner)
  {
  if( !scaner->tokenNeed( '{', QStringLiteral("No apperance") ) )
    return;

  while( !scaner->matchToken('}') ) {
    if( scaner->isEndOfScan() ) {
      scaner->error( QStringLiteral("Uncompleted apperance") );
      return;
      }
    if( scaner->isError() )
      return;
    QString nodeType;
    if( !scaner->tokenNeedValue( 'n', nodeType, QStringLiteral("Need apperance node") ) )
      return;
    if( nodeType == QStringLiteral("material") )
      mMaterial = parse2Declaration( scaner );
    else if( nodeType == QStringLiteral("texture") )
      mTexture = parse2Declaration( scaner );
    else if( nodeType == QStringLiteral("textureTransform") )
      mTextureTransform = parse2Declaration( scaner );
    else {
      scaner->error( QStringLiteral("Illegal node type for apperance %1").arg(nodeType) );
      return;
      }
    }
  }



