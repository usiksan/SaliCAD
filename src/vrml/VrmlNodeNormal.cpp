#include "VrmlNodeNormal.h"
#include "SdScanerVrml.h"





bool VrmlNodeNormal::parse(SdScanerVrml *scaner, const QString &fieldType)
  {
  if( fieldType == QStringLiteral("vector") )
    scaner->parseVectorTable( mVectorList, QStringLiteral("Waiting vector list") );
  else return false;

  return true;
  }
