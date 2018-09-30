/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Layer property
*/
#include "SdPropLayer.h"
#include "SdEnvir.h"

SdPropLayer::SdPropLayer() :
  mLayer( sdEnvir->getLayer(LID0_INVISIBLE) ),
  mValue(NoValue)
  {

  }



SdPropLayer::SdPropLayer(QString id) :
  mLayer( sdEnvir->getLayer(id) ),
  mValue(OneValue)
  {

  }




bool SdPropLayer::isVisible(bool otherSide) const
  {
  SdLayer *lay = layer( otherSide );
  if( lay )
    return lay->isVisible();
  return false;
  }




bool SdPropLayer::isEdited() const
  {
  SdLayer *lay = layer( false );
  if( lay )
    return lay->isEdited();
  return false;
  }



void SdPropLayer::set(const QString id)
  {
  if( id.isEmpty() ) {
    mValue = NoValue;
    mLayer = sdEnvir->getLayer(LID0_INVISIBLE);
    }
  else {
    mValue = OneValue;
    mLayer = sdEnvir->getLayer(id);
    }
  }





SdLayer *SdPropLayer::layer(bool otherSide) const
  {
  if( mValue == OneValue ) {
    if( otherSide ) return mLayer->pair();
    return mLayer;
    }
  return nullptr;
  }





void SdPropLayer::append(SdPropLayer p)
  {
  //If source not assigned then nothing appended
  if( p.mValue == NoValue )
    return;

  if( mValue == NoValue ) {
    //If there yet nothing assigned then copy source
    mValue = p.mValue;
    mLayer = p.mLayer;
    }
  else if( mValue == OneValue ) {
    if( p.mValue != OneValue || p.mLayer != mLayer )
      mValue = AllValue;
    }
  }







void SdPropLayer::setLayerUsage() const
  {
  if( mValue == OneValue && mLayer != nullptr ) {
    mLayer->setUsage();
    mLayer->pair()->setUsage();
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

