#include "VrmlColor.h"
#include "SdScanerVrml.h"

#include <QRgb>

void VrmlColor::parse(SdScanerVrml *scaner)
  {
  if( !scaner->tokenNeedValueFloat( 'f', mRed, QStringLiteral("Need color red") ) ) return;
  if( !scaner->tokenNeedValueFloat( 'f', mGreen, QStringLiteral("Need color green") ) ) return;
  scaner->tokenNeedValueFloat( 'f', mBlue, QStringLiteral("Need color blue") );
  }



quint32 VrmlColor::toInt() const
  {
  return qRgb( mRed * 255, mGreen * 255, mBlue * 255 );
  }
