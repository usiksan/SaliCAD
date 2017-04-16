/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/
#include "SdUndoRecordPropLineAnd2Points.h"

SdUndoRecordPropLineAnd2Points::SdUndoRecordPropLineAnd2Points(SdPropLine *prop, SdPoint *p1, SdPoint *p2) :
  mProp(prop),
  mSrc1(p1),
  mSrc2(p2)
  {
  mProp->saveState( &mPropState );
  mPoint1 = (*p1);
  mPoint2 = (*p2);
  }


void SdUndoRecordPropLineAnd2Points::undo()
  {
  mProp->swapState( &mPropState );
  mSrc1->swap( &mPoint1 );
  mSrc2->swap( &mPoint2 );
  }
