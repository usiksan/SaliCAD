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
  if( mPartImp != nullptr )
    mPartImp->pinStatusGet( pinNumber, mPinState );
  }



void SdUndoRecordPartImpPin::undo()
  {
  if( mPartImp != nullptr ) {
    SdPartImpPin tmpPin;
    mPartImp->pinStatusGet( mPinNumber, tmpPin );
    mPartImp->pinStatusSet( mPinNumber, mPinState );
    mPinState = tmpPin;
    }
  }
