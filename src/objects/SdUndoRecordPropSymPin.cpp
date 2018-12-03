/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Undo record for symbol pin properties and point.
*/
#include "SdUndoRecordPropSymPin.h"



SdUndoRecordPropSymPin::SdUndoRecordPropSymPin(SdPropSymPin *prp, SdPoint *org) :
  SdUndoRecord(),
  mSrcProp(prp),
  mSrcOrigin(org)
  {
  //Save original values
  mSrcProp->saveState( &mProp );
  mOrigin = *mSrcOrigin;
  }



void SdUndoRecordPropSymPin::undo()
  {
  mSrcProp->swapState( &mProp );
  mOrigin.swap( mSrcOrigin );
  }

