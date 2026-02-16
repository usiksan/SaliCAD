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
  mProp(*prp),
  mSrcOrigin(org),
  mOrigin(*org)
  {
  }


void SdUndoRecordPropPartPin::undo()
  {
  mSrcProp->swap( mProp );
  mOrigin.swap( mSrcOrigin );
  }
