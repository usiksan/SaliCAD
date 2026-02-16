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
  mProp(*prp),
  mSrcOrigin(org),
  mOrigin(*org)
  {
  }



void SdUndoRecordPropSymPin::undo()
  {
  mSrcProp->swap( mProp );
  mOrigin.swap( mSrcOrigin );
  }

