#include "SdPropBarPartPin.h"

SdPropBarPartPin::SdPropBarPartPin(const QString title) :
  SdPropBar( title )
  {
  mPinType = new QComboBox();
  mPinType->setEditable(true);

  //on select other font
  connect( mPinType, static_cast<void(QComboBox::*)(int)>(&QComboBox::activated), [=](int index){
    Q_UNUSED(index)
    emit propChanged();
    });
  addWidget( mPinType );
  }




void SdPropBarPartPin::setPropPartPin(SdPropPartPin *propPartPin)
  {
  if( propPartPin ) {
    //Set current layer
    setSelectedLayer( propPartPin->mLayer.layer(false) );

    //TODO Set current pin type
    //mPinType->setCurrentIndex( propSymPin->mPinType.getValue() );
    }
  }




void SdPropBarPartPin::getPropPartPin(SdPropPartPin *propPartPin)
  {
  if( propPartPin ) {
    //Get current layer
    SdLayer *layer = getSelectedLayer();
    if( layer )
      propPartPin->mLayer = layer;

    //Get current pin type
    QString pinType = mPinType->currentText();
    if( !pinType.isEmpty() )
      propPartPin->mPinType = pinType;
    }
  }
