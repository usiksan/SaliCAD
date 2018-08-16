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

#include <QLineEdit>
#include <QToolButton>
#include <QDebug>


static SdStringHistory pinTypeHistory;



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
  for( const QString &str : pinTypeHistory )
    mPinType->addItem( str );
  mPinType->setMinimumWidth(180);


  //on select other pin type
  connect( mPinType, static_cast<void(QComboBox::*)(int)>(&QComboBox::activated), [=](int){
    pinTypeHistory.reorderComboBoxString( mPinType );
    emit propChanged();
    });
  connect( mPinType->lineEdit(), &QLineEdit::editingFinished, [=](){
    pinTypeHistory.reorderComboBoxString( mPinType );
    emit propChanged();
    });
  addWidget( mPinType );

  //Button to select pin type from default pad association table
  QToolButton *but = new QToolButton();
  but->setText( QStringLiteral("...") );
  connect( but, &QToolButton::clicked, this, [this] () {
    QString str = SdDPads::selectPinType(this);
    if( !str.isEmpty() ) {
      mPinType->setCurrentText( str );
      pinTypeHistory.reorderComboBoxString( mPinType );
      emit propChanged();
      }
    } );
  addWidget( but );
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
    qDebug() << "setPropPartPin" << propPartPin->mPinType.str();
    mPinType->setCurrentText( propPartPin->mPinType.str() );
    pinTypeHistory.reorderComboBoxString( mPinType );
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




