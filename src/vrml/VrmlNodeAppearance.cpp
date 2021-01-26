#include "VrmlNodeAppearance.h"
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
  if( mMaterial != nullptr ) delete mMaterial;
  if( mTexture != nullptr ) delete mTexture;
  if( mTextureTransform != nullptr ) delete mTextureTransform;
  }


void VrmlNodeAppearance::parse(SdScanerVrml *scaner)
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



