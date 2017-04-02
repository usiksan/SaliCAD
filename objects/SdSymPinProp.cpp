/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/

#include "SdSymPinProp.h"


void SdSymPinProp::operator =(const SdSymPinProp &sour)
  {
  mLayer   = sour.mLayer;
  mPinType = sour.mPinType;
  }

void SdSymPinProp::append(const SdSymPinProp &sour)
  {
  mLayer.append( sour.mLayer );
  mPinType.append( sour.mPinType );
  }

void SdSymPinProp::clear()
  {
  mLayer.clear();
  mPinType.clear();
  }

bool SdSymPinProp::match(const SdSymPinProp &prop)
  {
  return mLayer.match(prop.mLayer) && mPinType.match(prop.mPinType);
  }

void SdSymPinProp::write(QJsonObject &obj) const
  {
  mLayer.write( QStringLiteral("SymPinLayer"), obj );
  mPinType.write( QStringLiteral("SymPinType"), obj );
  }

void SdSymPinProp::read(const QJsonObject obj)
  {
  mLayer.read( QStringLiteral("SymPinLayer"), obj );
  mPinType.read( QStringLiteral("SymPinType"), obj );
  }

