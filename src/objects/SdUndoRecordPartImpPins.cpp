/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Undo record for implement part all pins.
*/
#include "SdUndoRecordPartImpPins.h"

SdUndoRecordPartImpPins::SdUndoRecordPartImpPins(SdPartImpPinTable *table) :
  SdUndoRecord(),
  mSource(table),
  mPins(*table)
  {

  }


void SdUndoRecordPartImpPins::undo()
  {
  mSource->swap( mPins );
  }
