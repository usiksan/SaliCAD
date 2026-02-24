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
#include "SdStringHistory.h"
#include "SdDPads.h"
#include "SdDPadMaster.h"

#include <QLineEdit>
#include <QToolButton>
#include <QDebug>


static SdStringHistory pinTypeHistory;



SdPropBarPartPin::SdPropBarPartPin(const QString title) :
  SdPropBarLay( title )
  {
  mPinSide = new QComboBox();
  //Fill side variants
  mPinSide->addItems( {tr("---"), tr("Top smd"), tr("Bottom smd"), tr("Through")} );
  //on select other pin side
  connect( mPinSide, static_cast<void(QComboBox::*)(int)>(&QComboBox::activated), [=](int index){
    Q_UNUSED(index)
    emit propChanged();
    });
  addWidget( mPinSide );

  mPinType = new QComboBox();
  mPinType->setEditable(true);
  for( const QString &str : pinTypeHistory )
    mPinType->addItem( str );
  mPinType->setMinimumWidth(180);


  //on select other pin type
  connect( mPinType, static_cast<void(QComboBox::*)(int)>(&QComboBox::activated), this, &SdPropBarPartPin::setPinType );
  connect( mPinType->lineEdit(), &QLineEdit::editingFinished, this, &SdPropBarPartPin::setPinType );
  addWidget( mPinType );

  //Button to select pin type from default pad association table
  QToolButton *but = new QToolButton();
  but->setText( QStringLiteral("...") );
  connect( but, &QToolButton::clicked, this, [this] () {
    QString str = SdDPadMaster::build( mPinType->currentText(), this );
    if( str != mPinType->currentText() ) {
      mPinType->setCurrentText( str );
      setPinType();
      }
    } );
  addWidget( but );
  }




void SdPropBarPartPin::setPropPartPin(const SdPropComposerPartPin &propPartPin)
  {
  //Set current layer
  auto &propLayer = propPartPin.get<&SdPropPartPin::mLayer>();
  if( propLayer.isSingle() )
    updateEditObjectProp( dctPart, stmTop | stmBottom, propLayer.value().layer(false) );
  else
    updateEditObjectProp( dctPart, stmTop | stmBottom, nullptr );

  auto &propSide = propPartPin.get<&SdPropPartPin::mSide>();
  if( propSide.isSingle() ) {
    //Set current pin side
    switch( propSide.value().value() ) {
      case stmTop    : mPinSide->setCurrentIndex(1); break;
      case stmBottom : mPinSide->setCurrentIndex(2); break;
      case stmThrough  : mPinSide->setCurrentIndex(3); break;
      default: mPinSide->setCurrentIndex(0);
      }
    }
  else mPinSide->setCurrentIndex(0);

  auto &propType = propPartPin.get<&SdPropPartPin::mPinType>();
  //Set current pin type
  mPinType->setCurrentText( propType.isSingle() ? propType.value().string() : QString{} );
  pinTypeHistory.reorderComboBoxString( mPinType );
  }



void SdPropBarPartPin::getPropPartPin(SdPropComposerPartPin &propPartPin)
  {
  propPartPin.clear();
  //Store layer if setted
  SdLayer *layer = getSelectedLayer();
  if( layer )
    propPartPin.get<&SdPropPartPin::mLayer>().reset( SdPvLayer(layer) );

  //Get pin side
  auto &propSide = propPartPin.get<&SdPropPartPin::mSide>();
  int side = mPinSide->currentIndex();
  switch( side ) {
    case 1 : propSide.reset(stmTop); break;
    case 2 : propSide.reset(stmBottom); break;
    case 3 : propSide.reset(stmThrough); break;
    }

  //Get current pin type
  QString pinType = mPinType->currentText();
  if( !pinType.isEmpty() )
    propPartPin.get<&SdPropPartPin::mPinType>().reset( SdPvString(pinType) );
  }




void SdPropBarPartPin::setPropPartPin( const SdPropPartPin &propPartPin)
  {
  SdPropComposerPartPin composerPartPin;
  composerPartPin.reset( propPartPin );
  setPropPartPin( composerPartPin );
  }




void SdPropBarPartPin::getPropPartPin(SdPropPartPin &propPartPin)
  {
  SdPropComposerPartPin composerPartPin;
  getPropPartPin( composerPartPin );
  composerPartPin.store( propPartPin );
  }





void SdPropBarPartPin::setPinType()
  {
  //When pin type changed we need update smd or throw pin flag
  QString pinType = mPinType->currentText();
  if( !pinType.isEmpty() ) {
    SdPad pad;
    pad.parse( pinType );
    if( pad.isThrough() )
      mPinSide->setCurrentIndex(3);
    else
      mPinSide->setCurrentIndex(1);
    }
  pinTypeHistory.reorderComboBoxString( mPinType );
  emit propChanged();
  }




