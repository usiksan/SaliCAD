/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/
#include "SdUndoRecordWire.h"

SdUndoRecordWire::SdUndoRecordWire(SdPropLine *prop, SdPoint *p1, SdPoint *p2, bool *dot1, bool *dot2) :
  mProp(prop),
  mSrc1(p1),
  mSrc2(p2),
  mDotSrc1(dot1),
  mDotSrc2(dot2)
  {
  mProp->saveState( &mPropState );
  mPoint1 = (*p1);
  mPoint2 = (*p2);
  mDot1   = (*dot1);
  mDot2   = (*dot2);
  }


//Undo elementary operation.
void SdUndoRecordWire::undo()
  {
  mProp->swapState( &mPropState );
  mSrc1->swap( &mPoint1 );
  mSrc2->swap( &mPoint2 );

  bool t = *mDotSrc1;
  *mDotSrc1 = mDot1;
  mDot1 = t;

  t = *mDotSrc2;
  *mDotSrc2 = mDot2;
  mDot2 = t;
  }
