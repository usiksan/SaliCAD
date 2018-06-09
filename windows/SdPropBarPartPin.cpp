/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Properties bar for part pin
*/
#include "SdPropBarPartPin.h"
#include <QLineEdit>

SdPropBarPartPin::SdPropBarPartPin(const QString title) :
  SdPropBar( title )
  {
  mPinSide = new QComboBox();
  //Fill side variants
  mPinSide->addItems( {tr("---"), tr("Top smd"), tr("Bottom smd"), tr("Throw")} );
  //on select other pin side
  connect( mPinSide, static_cast<void(QComboBox::*)(int)>(&QComboBox::activated), [=](int index){
    Q_UNUSED(index)
    emit propChanged();
    });
  addWidget( mPinSide );

  mPinType = new QComboBox();
  mPinType->setEditable(true);
  mPinType->addItem( QString() );
  mPinType->setMinimumWidth(150);

  //on select other pin type
  connect( mPinType, static_cast<void(QComboBox::*)(int)>(&QComboBox::activated), [=](int index){
    Q_UNUSED(index)
    setPinType();
    });
  connect( mPinType->lineEdit(), &QLineEdit::editingFinished, [=](){
    setPinType();
    emit propChanged();
    });
  addWidget( mPinType );
  }




void SdPropBarPartPin::setPropPartPin(SdPropPartPin *propPartPin)
  {
  if( propPartPin ) {
    //Set current layer
    setSelectedLayer( propPartPin->mLayer.layer(false) );

    //Set current pin side
    int side = propPartPin->mSide.getValue();
    switch(side) {
      case stmTop    : mPinSide->setCurrentIndex(1); break;
      case stmBottom : mPinSide->setCurrentIndex(2); break;
      case stmThrow  : mPinSide->setCurrentIndex(3); break;
      default: mPinSide->setCurrentIndex(0);
      }

    //Set current pin type
    mPinType->setCurrentText( propPartPin->mPinType.str() );
    }
  }




void SdPropBarPartPin::getPropPartPin(SdPropPartPin *propPartPin)
  {
  if( propPartPin ) {
    //Get current layer
    SdLayer *layer = getSelectedLayer();
    if( layer )
      propPartPin->mLayer = layer;

    int side = mPinSide->currentIndex();
    switch( side ) {
      case 1 : propPartPin->mSide = stmTop; break;
      case 2 : propPartPin->mSide = stmBottom; break;
      case 3 : propPartPin->mSide = stmThrow; break;
      }

    //Get current pin type
    QString pinType = mPinType->currentText();
    if( !pinType.isEmpty() )
      propPartPin->mPinType = pinType;
    }
  }




void SdPropBarPartPin::setPinType()
  {
  QString pinType = mPinType->currentText();
  if( !pinType.isEmpty() ) {
    //Reorder previous pin type list
    int i = mPinType->findText( pinType );
    if( i >= 0 ) {
      //New pin type already present in previous list
      //move it to first position
      mPinType->removeItem(i);
      }
    mPinType->insertItem(1, pinType);
    }
  mPinType->setCurrentText( pinType );
  emit propChanged();
  }
