#include "VrmlNodeTransform.h"
#include "SdScanerVrml.h"

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







void VrmlNodeTransform::generateFaces(std::function<void (const QVector3DList &, const QVector3DList &, const VrmlNodeMaterial *)> appendFace) const
  {
  QMatrix4x4 m;
  m.translate( mTranslation.toVector3d() );
  m.translate( mCenter.toVector3d() );
  m.rotate( mRotation.mAngle, mRotation.vector3d() );
  m.rotate( mScaleOrientation.mAngle, mScaleOrientation.vector3d() );
  m.scale( mScale.toVector3d() );
  m.rotate( -mScaleOrientation.mAngle, mScaleOrientation.vector3d() );
  m.translate( mCenter.toVector3d() * -1.0 );

  VrmlNodeGroup::generateFaces( [m, appendFace] ( const QVector3DList &vertexList, const QVector3DList &normalList, const VrmlNodeMaterial *material ) {
    //Convert each vector from source vector
    QVector3DList dstVertex;
    for( auto vec : vertexList )
      dstVertex.append( m.map( vec )  );

    //Convert normal list
    QVector3DList dstNormal;
    for( auto vec : normalList )
      dstNormal.append( m.map( vec ).normalized() );

    //Append converted face
    appendFace( dstVertex, dstNormal, material );
    } );
  }
