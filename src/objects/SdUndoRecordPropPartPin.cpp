/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Undo record for part pin properties and point.
*/
#include "SdUndoRecordPropPartPin.h"

SdUndoRecordPropPartPin::SdUndoRecordPropPartPin(SdPropPartPin *prp, SdPoint *org) :
  SdUndoRecord(),
  mSrcProp(prp),
  mSrcOrigin(org)
  {
  //Save original values
  mSrcProp->saveState( &mProp );
  mOrigin = *mSrcOrigin;
  }


void SdUndoRecordPropPartPin::undo()
  {
  mSrcProp->swapState( &mProp );
  mOrigin.swap( mSrcOrigin );
  }
