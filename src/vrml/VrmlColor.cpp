#include "VrmlColor.h"
#include "SdScanerVrml.h"

VrmlColor::VrmlColor()
  {

  }

void VrmlColor::parse(SdScanerVrml *scaner)
  {
  if( !scaner->tokenNeedValueFloat( 'f', mRed, QStringLiteral("Need color red") ) ) return;
  if( !scaner->tokenNeedValueFloat( 'f', mGreen, QStringLiteral("Need color green") ) ) return;
  scaner->tokenNeedValueFloat( 'f', mBlue, QStringLiteral("Need color blue") );
  }
