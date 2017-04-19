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
  for( int i = 0; i < mPointList.count(); i++ )
    mPointList[i].swap( &((*mSrc)[i]) );
  }


