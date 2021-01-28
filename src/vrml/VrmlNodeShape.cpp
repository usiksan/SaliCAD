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



void VrmlNodeShape::generateFaces(std::function<void (const QVector3DList &, QVector3D, VrmlColor)> appendFace) const
  {
  //Get apperance color
  VrmlNodeAppearance *appearance = dynamic_cast<VrmlNodeAppearance*>(mApperance);
  VrmlColor appearanceColor = appearance == nullptr ? VrmlColor(-1.0) : appearance->color();
  //Generate geometry with apperance color
  if( mGeometry != nullptr )
    mGeometry->generateFaces( [appearanceColor,appendFace] (const QVector3DList &vector3dList, QVector3D normal, VrmlColor color) {
      appendFace( vector3dList, normal, appearanceColor.isValid() ? appearanceColor : color );
      });
  }
