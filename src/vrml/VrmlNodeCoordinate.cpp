#include "VrmlNodeCoordinate.h"
#include "SdScanerVrml.h"

VrmlNodeCoordinate::VrmlNodeCoordinate() :
  VrmlNode()
  {

  }


void VrmlNodeCoordinate::parse(SdScanerVrml *scaner)
  {
  mPointList.clear();
  if( !scaner->tokenNeed( '{', QStringLiteral("Need Coordinate block") ) )
    return;
  if( scaner->matchTokenValue( 'n', QStringLiteral("point") ) )
    scaner->parseVectorTable( mPointList, QStringLiteral("Waiting point list") );
  if( !scaner->tokenNeed( '}', QStringLiteral("Need Coordinate close block") ) )
    return;
  }

