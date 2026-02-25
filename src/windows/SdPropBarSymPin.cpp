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
  connect( mPinType, static_cast<void(QComboBox::*)(int)>(&QComboBox::activated), [this](int index){
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
  setSelectedLayer( propSymPin.layer().asSingleLayerOrNull(false) );

  //Set current pin type
  mPinType->setCurrentIndex( propSymPin.pinType().asSingleIntOrMinus() );
  }



void SdPropBarSymPin::getPropSymPin(SdPropComposerSymPin &propSymPin)
  {
  propSymPin.clear();
  //Store layer if setted
  propSymPin.layer().resetLayerNonNull( getSelectedLayer() );

  //Get current symbol pin type
  propSymPin.pinType().resetIntIfGeZero( mPinType->currentIndex() );
  }

