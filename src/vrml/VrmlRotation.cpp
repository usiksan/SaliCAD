#include "VrmlRotation.h"
#include "SdScanerVrml.h"

VrmlRotation::VrmlRotation(float vectorX, float vectorY, float vectorZ, float angle) :
  mVectorX(vectorX),
  mVectorY(vectorY),
  mVectorZ(vectorZ),
  mAngle(angle)
  {

  }

void VrmlRotation::parse(SdScanerVrml *scaner)
  {
  if( !scaner->tokenNeedValueFloat( 'f', mVectorX, QStringLiteral("Need rotation X value") ) ) return;
  if( !scaner->tokenNeedValueFloat( 'f', mVectorY, QStringLiteral("Need rotation Y value") ) ) return;
  if( !scaner->tokenNeedValueFloat( 'f', mVectorZ, QStringLiteral("Need rotation Z value") ) ) return;
  scaner->tokenNeedValueFloat( 'f', mAngle,   QStringLiteral("Need rotation angle value") );
  }
