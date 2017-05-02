/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Undo record for text properties and text string.
*/
#include "SdUndoRecordPropTextAndText.h"

SdUndoRecordPropTextAndText::SdUndoRecordPropTextAndText(SdPropText *prp, SdPoint *org, SdRect *r, QString *str) :
  SdUndoRecord(),
  mSrcProp(prp),
  mSrcOrigin(org),
  mSrcRect(r),
  mSrcString(str)
  {
  mSrcProp->saveState( &mPropState );
  mRect   = *r;
  mOrigin = *org;
  if( str )
    mString = *str;
  }


void SdUndoRecordPropTextAndText::undo()
  {
  mSrcProp->swapState( &mPropState );
  mRect = mSrcRect->swap( mRect );
  mOrigin.swap( mSrcOrigin );
  if( mSrcString )
    mSrcString->swap( mString );
  }
