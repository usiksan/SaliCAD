#include "VrmlNodeTransform.h"
#include "SdScanerVrml.h"

VrmlNodeTransform::VrmlNodeTransform() :
  VrmlNodeGroup()
  {

  }

VrmlNodeTransform::VrmlNodeTransform(const VrmlNodeTransform *transform) :
  VrmlNodeGroup( transform ),
  mCenter( transform->mCenter ),
  mRotation( transform->mRotation ),
  mScale( transform->mScale ),
  mScaleOrientation( transform->mScaleOrientation ),
  mTranslation( transform->mTranslation )
  {

  }




void VrmlNodeTransform::parse(SdScanerVrml *scaner)
  {
  if( !scaner->tokenNeed( '{', QStringLiteral("No transform") ) )
    return;

  while( !scaner->matchToken('}') ) {
    if( scaner->isEndOfScan() ) {
      scaner->error( QStringLiteral("Uncompleted transform") );
      return;
      }
    if( scaner->isError() )
      return;
    QString nodeType;
    if( !scaner->tokenNeedValue( 'n', nodeType, QStringLiteral("Need group node") ) )
      return;
    if( parse2GroupComponents( scaner, nodeType ) )
      continue;
    if( nodeType == QStringLiteral("center") )
      mCenter.parse( scaner );
    else if( nodeType == QStringLiteral("rotation") )
      mRotation.parse( scaner );
    else if( nodeType == QStringLiteral("scale") )
      mScale.parse( scaner );
    else if( nodeType == QStringLiteral("scaleOrientation") )
      mScaleOrientation.parse( scaner );
    else if( nodeType == QStringLiteral("translation") )
      mTranslation.parse( scaner );
    else {
      scaner->error( QStringLiteral("Undefined transform node %1").arg(nodeType) );
      return;
      }
    }
  }

