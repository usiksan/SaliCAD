#include "VrmlNodeApperance.h"
#include "SdScanerVrml.h"

VrmlNodeApperance::VrmlNodeApperance()
  {

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
    if( nodeType == QStringLiteral("Material") )
      mMaterial = parse2Node( scaner, nodeType );
    else if( nodeType == QStringLiteral("Texture") )
      mTexture = parse2Node( scaner, nodeType );
    else if( nodeType == QStringLiteral("TextureTransform") )
      mTextureTransform = parse2Node( scaner, nodeType );
    else {
      scaner->error( QStringLiteral("Illegal node type for apperance %1").arg(nodeType) );
      return;
      }
    }
  }



VrmlNode *VrmlNodeApperance::copy() const
  {
  VrmlNodeApperance *apperance = new VrmlNodeApperance();
  cloneNode( apperance );
  if( mMaterial )
    apperance->mMaterial = mMaterial->copy();
  if( mTexture )
    apperance->mTexture = mTexture->copy();
  if( mTextureTransform )
    apperance->mTextureTransform = mTextureTransform->copy();
  return apperance;
  }
