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
  mApperance(nullptr),
  mGeometry(nullptr)
  {
  mApperance = makeCopy( shape->mApperance );
  mGeometry  = makeCopy( shape->mGeometry );
  }


void VrmlNodeShape::parse(SdScanerVrml *scaner)
  {
  if( !scaner->tokenNeed( '{', QStringLiteral("No shape") ) )
    return;

  while( !scaner->matchToken('}') ) {
    if( scaner->isEndOfScan() ) {
      scaner->error( QStringLiteral("Uncompleted shape") );
      return;
      }
    if( scaner->isError() )
      return;
    QString nodeType;
    if( !scaner->tokenNeedValue( 'n', nodeType, QStringLiteral("Need shape node") ) )
      return;
    if( nodeType == QStringLiteral("apperance") )
      mApperance = parse2Declaration( scaner );
    else if( nodeType == QStringLiteral("geometry") )
      mGeometry = parse2Declaration( scaner );
    else {
      scaner->error( QStringLiteral("Undefined Shape node %1").arg(nodeType) );
      return;
      }
    }
  }



