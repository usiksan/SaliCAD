/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/

#include "SdPartPinProp.h"


void SdPartPinProp::operator =(const SdPartPinProp &sour)
  {
  mLayer   = sour.mLayer;
  mPinType = sour.mPinType;
  mSide    = sour.mSide;
  }

void SdPartPinProp::append(const SdPartPinProp &sour)
  {
  mLayer.append( sour.mLayer );
  mPinType.append( sour.mPinType );
  mSide.append( sour.mSide );
  }

void SdPartPinProp::clear()
  {
  mLayer.clear();
  mPinType.clear();
  mSide.clear();
  }

bool SdPartPinProp::match(const SdPartPinProp &prop)
  {
  return mLayer.match(prop.mLayer) && mPinType.match(prop.mPinType) && mSide.match(prop.mSide);
  }

void SdPartPinProp::write(QJsonObject &obj) const
  {
  mLayer.write( QStringLiteral("PartPinLayer"), obj );
  mPinType.write( QStringLiteral("PartPinType"), obj );
  mSide.write( QStringLiteral("PartPinSide"), obj );
  }

void SdPartPinProp::read(const QJsonObject obj)
  {
  mLayer.read( QStringLiteral("PartPinLayer"), obj );
  mPinType.read( QStringLiteral("PartPinType"), obj );
  mSide.read( QStringLiteral("PartPinSide"), obj );
  }

