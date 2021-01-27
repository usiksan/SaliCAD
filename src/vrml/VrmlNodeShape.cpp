#include "VrmlNodeShape.h"
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
