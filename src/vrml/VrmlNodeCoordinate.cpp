#include "VrmlNodeCoordinate.h"
#include "SdScanerVrml.h"

VrmlNodeCoordinate::VrmlNodeCoordinate() :
  VrmlNode()
  {

  }



bool VrmlNodeCoordinate::parse(SdScanerVrml *scaner, const QString &fieldType)
  {
  if( fieldType == QStringLiteral("point") ) {
    mPointList.clear();
    scaner->parseVectorTable( mPointList, QStringLiteral("Waiting point list") );
    return true;
    }
  return false;
  }
