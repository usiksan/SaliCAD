/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/

#include "SdPropPartPin.h"


void SdPropPartPin::operator =(const SdPropPartPin &sour)
  {
  mLayer   = sour.mLayer;
  mPinType = sour.mPinType;
  mSide    = sour.mSide;
  }

void SdPropPartPin::append(const SdPropPartPin &sour)
  {
  mLayer.append( sour.mLayer );
  mPinType.append( sour.mPinType );
  mSide.append( sour.mSide );
  }

void SdPropPartPin::clear()
  {
  mLayer.clear();
  mPinType.clear();
  mSide.clear();
  }

bool SdPropPartPin::match(const SdPropPartPin &prop)
  {
  return mLayer.match(prop.mLayer) && mPinType.match(prop.mPinType) && mSide.match(prop.mSide);
  }

void SdPropPartPin::write(QJsonObject &obj) const
  {
  mLayer.write( QStringLiteral("PartPinLayer"), obj );
  mPinType.write( QStringLiteral("PartPinType"), obj );
  mSide.write( QStringLiteral("PartPinSide"), obj );
  }

void SdPropPartPin::read(const QJsonObject obj)
  {
  mLayer.read( QStringLiteral("PartPinLayer"), obj );
  mPinType.read( QStringLiteral("PartPinType"), obj );
  mSide.read( QStringLiteral("PartPinSide"), obj );
  }

void SdPropPartPin::saveState(SdPropPartPinState *dst)
  {

  }

