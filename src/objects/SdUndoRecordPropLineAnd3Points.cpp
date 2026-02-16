/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Undo record for line properties and 2 points.
*/
#include "SdUndoRecordPropLineAnd3Points.h"

SdUndoRecordPropLineAnd3Points::SdUndoRecordPropLineAnd3Points(SdPropLine *prop, SdPoint *p1, SdPoint *p2, SdPoint *p3) :
  mProp(prop),
  mSrc1(p1),
  mSrc2(p2),
  mSrc3(p3)
  {
  if( mProp )
    mPropState = *prop;
  if( p1 )
    mPoint1 = (*p1);
  if( p2 )
    mPoint2 = (*p2);
  if( p3 )
    mPoint3 = (*p3);
  }


void SdUndoRecordPropLineAnd3Points::undo()
  {
  if( mProp )
    mProp->swap( mPropState );
  if( mSrc1 )
    mSrc1->swap( &mPoint1 );
  if( mSrc2 )
    mSrc2->swap( &mPoint2 );
  if( mSrc3 )
    mSrc3->swap( &mPoint3 );
  }
