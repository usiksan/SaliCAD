#include "VrmlNodeShape.h"
#include "VrmlNodeAppearance.h"
#include "SdScanerVrml.h"

VrmlNodeShape::VrmlNodeShape() :
  VrmlNode(),
  mApperance(nullptr),
  mGeometry(nullptr)
  {

  }



VrmlNodeShape::VrmlNodeShape(const VrmlNodeShape *shape) :
  VrmlNode(shape),
  mApperance( makeCopy( shape->mApperance ) ),
  mGeometry( makeCopy( shape->mGeometry ) )
  {
  }



VrmlNodeShape::~VrmlNodeShape()
  {
  deleteNode( mApperance );
  deleteNode( mGeometry );
  }



bool VrmlNodeShape::parse(SdScanerVrml *scaner, const QString &fieldType)
  {
  if( fieldType == QStringLiteral("appearance") )
    mApperance = parse2Declaration( scaner );

  else if( fieldType == QStringLiteral("geometry") )
    mGeometry = parse2Declaration( scaner );

  else return false;

  return true;
  }





void VrmlNodeShape::generateFaces(std::function<void (const QVector3DList &, const QVector3DList &, const VrmlNodeMaterial *)> appendFace) const
  {
  //Get apperance color
  VrmlNodeAppearance *appearance = dynamic_cast<VrmlNodeAppearance*>(mApperance);
  VrmlNodeMaterial *material = appearance == nullptr ? nullptr : appearance->material();
  //Generate geometry with apperance color
  if( mGeometry != nullptr )
    mGeometry->generateFaces( [material,appendFace] (const QVector3DList &vector3dList, const QVector3DList &normalList, const VrmlNodeMaterial *childMaterial ) {
      appendFace( vector3dList, normalList, childMaterial == nullptr ? material : childMaterial );
      });
  }
