/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Properties bar for symbol pin
*/
#include "SdPropBarSymPin.h"
#include <QComboBox>

SdPropBarSymPin::SdPropBarSymPin(const QString title) :
  SdPropBarLay( title )
  {
  mPinType = new QComboBox();
  mPinType->setEditable(false);
  //Fill pinType list with available types
  mPinType->addItem( tr("undefined") );
  mPinType->addItem( tr("input") );
  mPinType->addItem( tr("output") );
  mPinType->addItem( tr("in-out") );
  mPinType->addItem( tr("analog in") );
  mPinType->addItem( tr("analog out") );
  mPinType->addItem( tr("analog") );
  mPinType->addItem( tr("power") );

  //on select other font
  connect( mPinType, static_cast<void(QComboBox::*)(int)>(&QComboBox::activated), [=](int index){
    Q_UNUSED(index)
    emit propChanged();
    });
  addWidget( mPinType );

  }




void SdPropBarSymPin::setPropSymPin( const SdPropSymPin &propSymPin )
  {
  SdPropComposerSymPin propComposerSymPin;
  propComposerSymPin.reset( propSymPin );
  setPropSymPin( propComposerSymPin );
  }





void SdPropBarSymPin::getPropSymPin(SdPropSymPin &propSymPin)
  {
  SdPropComposerSymPin propComposerSymPin;
  getPropSymPin( propComposerSymPin );
  propComposerSymPin.store( propSymPin );
  }



void SdPropBarSymPin::setPropSymPin(const SdPropComposerSymPin &propSymPin)
  {
  //Set current layer
  auto &propLayer = propSymPin.get<&SdPropSymPin::mLayer>();
  if( propLayer.isSingle() )
    updateEditObjectProp( dctSymbol, stmThrough, propLayer.value().layer(false) );
  else
    updateEditObjectProp( dctSymbol, stmThrough, nullptr );

  //Set current pin type
  auto &pinType = propSymPin.get<&SdPropSymPin::mPinType>();
  mPinType->setCurrentIndex( pinType.isSingle() ? pinType.value().value() : -1 );
  }



void SdPropBarSymPin::getPropSymPin(SdPropComposerSymPin &propSymPin)
  {
  propSymPin.clear();
  //Store layer if setted
  SdLayer *layer = getSelectedLayer();
  if( layer )
    propSymPin.get<&SdPropSymPin::mLayer>().reset( SdPvLayer(layer) );

  //Get current font
  int pinType = mPinType->currentIndex();
  if( pinType >= 0 )
    propSymPin.get<&SdPropSymPin::mPinType>().reset( SdPvInt(pinType) );
  }

