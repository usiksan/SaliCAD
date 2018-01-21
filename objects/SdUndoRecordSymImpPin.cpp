/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Undo record for implement symbol pin properties and point.
*/
#include "SdUndoRecordSymImpPin.h"
#include "SdGraphSymImp.h"

SdUndoRecordSymImpPin::SdUndoRecordSymImpPin(SdGraphSymImp *imp, QString pinName) :
  SdUndoRecord(),
  mSymImp(imp),
  mPinName(pinName)
  {
  if( mSymImp != nullptr ) {
    SdSymImpPin *pin = mSymImp->getPin( mPinName );
    if( pin != nullptr )
      mPinState = *pin;
    else
      mSymImp = nullptr;
    }
  }




void SdUndoRecordSymImpPin::undo()
  {
  if( mSymImp != nullptr ) {
    SdSymImpPin *pin = mSymImp->getPin( mPinName );
    SdSymImpPin tmpPin;
    tmpPin = *pin;
    *pin = mPinState;
    mPinState = tmpPin;
    }
  }

