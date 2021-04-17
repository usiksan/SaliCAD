#include "VrmlNode1IndexedFaceSet.h"
#include "SdScanerVrml.h"

VrmlNode1IndexedFaceSet::VrmlNode1IndexedFaceSet()
  {

  }


bool VrmlNode1IndexedFaceSet::parse(SdScanerVrml *scaner, const QString &fieldType)
  {
  if( fieldType == QStringLiteral("coordIndex") )
    scaner->parseInt32Table( mCoordIndex, QStringLiteral("Need coord index") );

  else if( fieldType == QStringLiteral("materialIndex") )
    scaner->parseInt32Table( mMaterialIndex, QStringLiteral("Need material index") );

  else if( fieldType == QStringLiteral("normalIndex") )
    scaner->parseInt32Table( mNormalIndex, QStringLiteral("Need normal index") );

  else return false;

  return true;
  }
