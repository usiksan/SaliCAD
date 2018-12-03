#include "SdUndoRecordPropLineAndPointsList.h"

SdUndoRecordPropLineAndPointsList::SdUndoRecordPropLineAndPointsList(SdPropLine *prp, SdPointList *src) :
  SdUndoRecord(),
  mProp(prp),
  mSrc(src),
  mPointList(*src)
  {
  mProp->saveState( &mPropState );
  }




void SdUndoRecordPropLineAndPointsList::undo()
  {
  mProp->swapState( &mPropState );
  mPointList.swap( *mSrc );
  }


