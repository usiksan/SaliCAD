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
#include "SdUtil.h"

SdUndoRecordPropLinePointInt::SdUndoRecordPropLinePointInt(SdPropLine *prop, SdPoint *p1, int *val) :
  mProp(prop),
  mPropState(*prop),
  mSrc1(p1),
  mPoint1(*p1),
  mSrc2(val),
  mValue2(*val)
  {
  }




//Undo elementary operation.
void SdUndoRecordPropLinePointInt::undo()
  {
  mProp->swap( mPropState );
  mSrc1->swap( &mPoint1 );
  SdUtil::swap( mValue2, mSrc2 );
  }

