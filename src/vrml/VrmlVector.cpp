#include "VrmlVector.h"
#include "SdScanerVrml.h"

VrmlVector::VrmlVector()
  {

  }

void VrmlVector::parse(SdScanerVrml *scaner)
  {
  mX = scaner->tokenNeedValueFloat('f');
  if( scaner->isEndOfScan() ) return;
  mY = scaner->tokenNeedValueFloat('f');
  if( scaner->isEndOfScan() ) return;
  mZ = scaner->tokenNeedValueFloat('f');
  }
