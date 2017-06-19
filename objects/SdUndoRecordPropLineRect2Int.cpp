/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Undo record for line properties, rect and 2 ints.
*/
#include "SdUndoRecordPropLineRect2Int.h"

SdUndoRecordPropLineRect2Int::SdUndoRecordPropLineRect2Int(SdPropLine *prp, SdRect *r, int *v1, int *v2) :
  SdUndoRecord(),
  mProp(prp),
  mRectSrc(r),
  mValSrc1(v1),
  mValSrc2(v2)
  {
  if( mProp )
    mProp->saveState( &mPropState );
  if( mRectSrc )
    mRect = (*mRectSrc);
  if( mValSrc1 )
    mVal1 = *mValSrc1;
  if( mValSrc2 )
    mVal2 = *mValSrc2;
  }




void SdUndoRecordPropLineRect2Int::undo()
  {
  if( mProp )
    mProp->swapState( &mPropState );
  if( mRectSrc )
    mRectSrc->swap( mRect );
  if( mValSrc1 ) {
    int tmp = *mValSrc1;
    *mValSrc1 = mVal1;
    mVal1 = tmp;
    }
  if( mValSrc2 ) {
    int tmp = *mValSrc2;
    *mValSrc2 = mVal2;
    mVal2 = tmp;
    }
  }

