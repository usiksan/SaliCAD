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
  mPropState(*prp),
  mSrcOrigin(org),
  mSrcRect(r),
  mSrcString(str)
  {
  if( org )
    mOrigin = *org;
  if( r )
    mRect   = *r;
  if( str )
    mString = *str;
  }


void SdUndoRecordPropTextAndText::undo()
  {
  mSrcProp->swap( mPropState );
  if( mSrcOrigin )
    mOrigin.swap( mSrcOrigin );
  if( mSrcRect )
    mRect = mSrcRect->swap( mRect );
  if( mSrcString )
    mSrcString->swap( mString );
  }
