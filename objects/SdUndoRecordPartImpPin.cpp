/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/
#include "SdUndoRecordPartImpPin.h"

SdUndoRecordPartImpPin::SdUndoRecordPartImpPin(SdGraphPartImp *partImp, QString pinNumber) :
  SdUndoRecord(),
  mPartImp(partImp),
  mPinNumber(pinNumber)
  {
  if( mPartImp != nullptr ) {
    SdPartImpPin *pin = mPartImp->getPin( pinNumber );
    if( pin != nullptr )
      mPinState = *pin;
    else
      mPartImp = nullptr;
    }
  }



void SdUndoRecordPartImpPin::undo()
  {
  if( mPartImp != nullptr ) {
    SdPartImpPin *pin = mPartImp->getPin( mPinNumber );
    SdPartImpPin tmpPin;
    tmpPin = *pin;
    *pin = mPinState;
    mPinState = tmpPin;
    }
  }
