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
  if( mSymImp != nullptr )
    mSymImp->pinStatusGet( mPinName, mPinState );
  }




void SdUndoRecordSymImpPin::undo()
  {
  if( mSymImp != nullptr ) {
    SdSymImpPin tmpPin;
    mSymImp->pinStatusGet( mPinName, tmpPin );
    mSymImp->pinStatusSet( mPinName, mPinState );
    mPinState = tmpPin;
    }
  }

