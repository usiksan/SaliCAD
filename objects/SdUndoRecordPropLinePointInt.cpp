/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Undo record for line properties, one point and int.
*/
#include "SdUndoRecordPropLinePointInt.h"

SdUndoRecordPropLinePointInt::SdUndoRecordPropLinePointInt(SdPropLine *prop, SdPoint *p1, int *val) :
  mProp(prop),
  mSrc1(p1),
  mSrc2(val)
  {
  mProp->saveState( &mPropState );
  mPoint1 = (*p1);
  mValue2 = (*val);
  }




//Undo elementary operation.
void SdUndoRecordPropLinePointInt::undo()
  {
  mProp->swapState( &mPropState );
  mSrc1->swap( &mPoint1 );
  int tmp = *mSrc2;
  *mSrc2 = mValue2;
  mValue2 = tmp;
  }

