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

void SdPropLayer::set(const QString id)
  {
  if( id.isEmpty() ) {
    mValue = NoValue;
    mLayer = 0;
    }
  else {
    mValue = OneValue;
    mLayer = sdEnvir->getLayer(id);
    }
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



SdLayer *SdPropLayer::swap(SdLayer *src)
  {
  SdLayer *lay = mLayer;
  mLayer = src;
  if( src ) mValue = OneValue;
  return lay;
  }
