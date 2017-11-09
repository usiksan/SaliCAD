/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Undo record for part implementation in plate
*/
#include "SdUndoRecordPartImp.h"
#include "SdUtil.h"

SdUndoRecordPartImp::SdUndoRecordPartImp(SdPoint *origin, SdPropPartImp *imp, int *logNumber, SdRect *over, QString *prefix, SdPropText *identProp, SdPoint *identOrigin, SdPoint *identPos, SdRect *identRect) :
  SdUndoRecord(),
  mOriginSrc(origin),
  mOriginValue(*origin),
  mPropSrc(imp),
  mLogNumberSrc(logNumber),
  mLogNumberValue(*logNumber),
  mOverSrc(over),
  mOver(*over),
  mPrefixSrc(prefix),
  mPrefix(*prefix),
  mIdentPropSrc(identProp),
  mIdentOriginSrc(identOrigin),
  mIdentOrigin(*identOrigin),
  mIdentPosSrc(identPos),
  mIdentPos(*identPos),
  mIdentRectSrc(identRect),
  mIdentRect(*identRect)
  {
  imp->saveState( &mProp );
  identProp->saveState( &mIdentProp );
  }




void SdUndoRecordPartImp::undo()
  {
  mOriginValue.swap( mOriginSrc );
  mPropSrc->swapState( &mProp );

  SdUtil::swapInt( mLogNumberValue, mLogNumberSrc );

  mOver = mOverSrc->swap( mOver );
  mPrefixSrc->swap( mPrefix );
  mIdentPropSrc->swapState( &mIdentProp );
  mIdentOrigin.swap( mIdentOriginSrc );
  mIdentPos.swap( mIdentPosSrc );
  mIdentRect = mIdentRectSrc->swap( mIdentRect );
  }
