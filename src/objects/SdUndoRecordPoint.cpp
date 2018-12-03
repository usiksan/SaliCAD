#include "SdUndoRecordPoint.h"

SdUndoRecordPoint::SdUndoRecordPoint(SdPoint *src) :
  SdUndoRecord(),
  mSource(src)
  {
  mPoint = *mSource;
  }


void SdUndoRecordPoint::undo()
  {
  mPoint.swap( mSource );
  }

