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
  SdPropBar( title )
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




void SdPropBarSymPin::setPropSymPin(SdPropSymPin *propSymPin)
  {
  if( propSymPin ) {
    //Set current layer
    updateEditObjectProp( dctSymbol, stmThrough, propSymPin->mLayer.layer(false) );

    //Set current pin type
    mPinType->setCurrentIndex( propSymPin->mPinType.getValue() );
    }
  }





void SdPropBarSymPin::getPropSymPin(SdPropSymPin *propSymPin)
  {
  if( propSymPin ) {
    //Get current layer
    SdLayer *layer = getSelectedLayer();
    if( layer )
      propSymPin->mLayer = layer;

    //Get current font
    int pinType = mPinType->currentIndex();
    if( pinType >= 0 )
      propSymPin->mPinType = pinType;
    }
  }

