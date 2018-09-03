/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Undo record for via
*/
#include "SdUndoRecordVia.h"

SdUndoRecordVia::SdUndoRecordVia(SdPropString *srcPadType, SdPoint *srcPosition) :
  SdUndoRecord(),
  mSrcPadType(srcPadType),
  mSrcPosition(srcPosition)
  {
  mPadType  = mSrcPadType->str();
  mPosition = *mSrcPosition;
  }




void SdUndoRecordVia::undo()
  {
  mSrcPadType->swap( &mPadType );
  mSrcPosition->swap( &mPosition );
  }
