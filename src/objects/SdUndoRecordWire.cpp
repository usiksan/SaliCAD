/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Undo record for wire in schematic sheets
*/
#include "SdUndoRecordWire.h"
#include "SdUtil.h"

SdUndoRecordWire::SdUndoRecordWire(SdPropLine *prop, SdPoint *p1, SdPoint *p2, bool *dot1, bool *dot2) :
  mProp(prop),
  mPropState( *prop ),
  mSrc1(p1),
  mPoint1(*p1),
  mSrc2(p2),
  mPoint2(*p2),
  mDotSrc1(dot1),
  mDot1(*dot1),
  mDotSrc2(dot2),
  mDot2(*dot2)
  {
  }


//Undo elementary operation.
void SdUndoRecordWire::undo()
  {
  mProp->swap( mPropState );
  mSrc1->swap( &mPoint1 );
  mSrc2->swap( &mPoint2 );

  SdUtil::swap( mDot1, mDotSrc1 );
  SdUtil::swap( mDot2, mDotSrc2 );

  }
