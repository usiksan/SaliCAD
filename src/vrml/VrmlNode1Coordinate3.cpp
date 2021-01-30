#include "VrmlNode1Coordinate3.h"
#include "SdScanerVrml.h"

VrmlNode1Coordinate3::VrmlNode1Coordinate3()
  {

  }


bool VrmlNode1Coordinate3::parse(SdScanerVrml *scaner, const QString &fieldType)
  {

  if( fieldType == QStringLiteral("point") )
    scaner->parseVectorTable( mPointTable, QStringLiteral("Need point table") );

  else return false;

  return true;
  }

