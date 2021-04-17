#include "VrmlVector.h"
#include "SdScanerVrml.h"


void VrmlVector::parse(SdScanerVrml *scaner)
  {
  if( !scaner->tokenNeedValueFloat( 'f', mX, QStringLiteral("Need vector X") ) ) return;
  if( !scaner->tokenNeedValueFloat( 'f', mY, QStringLiteral("Need vector X") ) ) return;
  scaner->tokenNeedValueFloat( 'f', mZ, QStringLiteral("Need vector X") );
  }
