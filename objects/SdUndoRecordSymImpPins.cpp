/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Undo record for implement symbol all pins.
*/
#include "SdUndoRecordSymImpPins.h"

SdUndoRecordSymImpPins::SdUndoRecordSymImpPins(SdSymImpPinTable *pins) :
  SdUndoRecord(),
  mSource(pins),
  mPins(*pins)
  {

  }



void SdUndoRecordSymImpPins::undo()
  {
  mSource->swap( mPins );
  }
