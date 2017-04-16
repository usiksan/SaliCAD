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

SdUndoRecordPropTextAndText::SdUndoRecordPropTextAndText(SdPropText *prp, SdRect *r, QString *str) :
  SdUndoRecord(),
  mSrcProp(prp),
  mSrcRect(r),
  mSrcString(str)
  {
  mSrcProp->saveState( &mPropState );
  mRect   = *r;
  mString = *str;
  }


void SdUndoRecordPropTextAndText::undo()
  {
  mSrcProp->swapState( &mPropState );
  mRect = mSrcRect->swap( mRect );
  mSrcString->swap( mString );
  }
