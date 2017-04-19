/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/

#include "SdPropSymPin.h"


void SdPropSymPin::operator =(const SdPropSymPin &sour)
  {
  mLayer   = sour.mLayer;
  mPinType = sour.mPinType;
  }




void SdPropSymPin::append(const SdPropSymPin &sour)
  {
  mLayer.append( sour.mLayer );
  mPinType.append( sour.mPinType );
  }




void SdPropSymPin::clear()
  {
  mLayer.clear();
  mPinType.clear();
  }




bool SdPropSymPin::match(const SdPropSymPin &prop)
  {
  return mLayer.match(prop.mLayer) && mPinType.match(prop.mPinType);
  }




void SdPropSymPin::write(QJsonObject &obj) const
  {
  mLayer.write( QStringLiteral("SymPinLayer"), obj );
  mPinType.write( QStringLiteral("SymPinType"), obj );
  }




void SdPropSymPin::read(const QJsonObject obj)
  {
  mLayer.read( QStringLiteral("SymPinLayer"), obj );
  mPinType.read( QStringLiteral("SymPinType"), obj );
  }




void SdPropSymPin::saveState(SdPropSymPinState *dst)
  {
  dst->mPinType = mPinType.getValue();
  dst->mLayer   = mLayer.layer();
  }




void SdPropSymPin::swapState(SdPropSymPinState *src)
  {
  src->mPinType = mPinType.swap( src->mPinType );
  src->mLayer   = mLayer.swap( src->mLayer );
  }


