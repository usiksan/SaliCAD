#include "VrmlNodeTransform.h"
#include "SdScanerVrml.h"
#include "3dr/Sd3drModel.h"

#include <QMatrix4x4>



VrmlNodeTransform::VrmlNodeTransform() :
  VrmlNodeGroup(),
  mCenter(),
  mRotation( 0.0, 0.0, 1.0, 0.0 ),
  mScale( 1.0, 1.0, 1.0 ),
  mScaleOrientation( 0.0, 0.0, 1.0, 0.0 ),
  mTranslation()
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







void VrmlNodeTransform::generateFaces(Sd3drModel *model, Sd3drInstance &instance, Sd3drBody &body) const
  {
  QMatrix4x4 m;
  m.translate( mTranslation.toVector3d() );
  m.translate( mCenter.toVector3d() );
  m.rotate( mRotation.mAngle, mRotation.vector3d() );
  m.rotate( mScaleOrientation.mAngle, mScaleOrientation.vector3d() );
  m.scale( mScale.toVector3d() );
  m.rotate( -mScaleOrientation.mAngle, mScaleOrientation.vector3d() );
  m.translate( mCenter.toVector3d() * -1.0 );

  instance.clear();
  instance.addCopy( m );

  VrmlNodeGroup::generateFaces( model, instance, body );
  model->instanceAppend( instance );
  }
