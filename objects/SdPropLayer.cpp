/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/
#include "SdPropLayer.h"
#include "SdEnvir.h"

SdPropLayer::SdPropLayer() :
  mLayer( sdEnvir->getLayer(LID0_INVISIBLE) ),
  mValue(NoValue)
  {

  }

SdLayer *SdPropLayer::layer(bool otherSide) const
  {
  if( otherSide ) return mLayer->pair();
  return mLayer;
  }

void SdPropLayer::append(SdPropLayer p)
  {
  if( mValue == NoValue ) {
    mValue = p.mValue;
    mLayer = p.mLayer;
    }
  else if( mValue == OneValue ) {
    if( p.mValue != OneValue || p.mLayer != mLayer )
      mValue = AllValue;
    }
  }

void SdPropLayer::read(const QString name, const QJsonObject obj)
  {
  mValue = OneValue;
  mLayer = sdEnvir->getLayer( obj.value(name).toString() );
  }

QDataStream& operator >> ( QDataStream &is, SdPropLayer &p ) {
  QString id;
  is >> id;
  p.mLayer = sdEnvir->getLayer( id );
  p.mValue = p.mLayer ? SdPropLayer::OneValue : SdPropLayer::NoValue;
  return is;
  }

QDataStream& operator << ( QDataStream &os, const SdPropLayer &p ) {
  if( p.mValue == SdPropLayer::OneValue )
    os << p.mLayer->id();
  else
    os << QString();
  return os;
  }
