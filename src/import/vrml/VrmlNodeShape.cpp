#include "VrmlNodeShape.h"
#include "VrmlNodeAppearance.h"
#include "SdScanerVrml.h"
#include "3dr/Sd3drModel.h"

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





void VrmlNodeShape::generateFaces(Sd3drModel *model, Sd3drInstance &instance, Sd3drBody &body) const
  {
  //Get apperance color
  VrmlNodeAppearance *appearance = dynamic_cast<VrmlNodeAppearance*>(mApperance);
  VrmlNodeMaterial *material = appearance == nullptr ? nullptr : appearance->material();
  body.clear();
  if( material != nullptr )
    body.colorListSet( material->to3dMaterial() );
  //Generate geometry with apperance color
  if( mGeometry != nullptr ) {
    mGeometry->generateFaces( model, instance, body );
    instance.add( body );
    body.clear();
    }
  }
