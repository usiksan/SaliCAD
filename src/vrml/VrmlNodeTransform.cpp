#include "VrmlNodeTransform.h"
#include "SdScanerVrml.h"

VrmlNodeTransform::VrmlNodeTransform()
  {

  }


VrmlNode *VrmlNodeTransform::copy() const
  {
  VrmlNodeTransform *transform = new VrmlNodeTransform();
  cloneNodeGroup( transform );
  transform->mCenter           = mCenter;
  transform->mRotation         = mRotation;
  transform->mScale            = mScale;
  transform->mScaleOrientation = mScaleOrientation;
  transform->mTranslation      = mTranslation;

  return transform;
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
