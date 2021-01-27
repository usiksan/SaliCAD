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







bool VrmlNodeTransform::parse(SdScanerVrml *scaner, const QString &fieldType)
  {
  if( !VrmlNodeGroup::parse( scaner, fieldType ) ) {

    if( fieldType == QStringLiteral("center") )
      mCenter.parse( scaner );

    else if( fieldType == QStringLiteral("rotation") )
      mRotation.parse( scaner );

    else if( fieldType == QStringLiteral("scale") )
      mScale.parse( scaner );

    else if( fieldType == QStringLiteral("scaleOrientation") )
      mScaleOrientation.parse( scaner );

    else if( fieldType == QStringLiteral("translation") )
      mTranslation.parse( scaner );

    else return false;
    }
  return true;
  }
