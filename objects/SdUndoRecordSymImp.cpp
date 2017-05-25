/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Undo record for symbol implementation in sheet
*/
#include "SdUndoRecordSymImp.h"


SdUndoRecordSymImp::SdUndoRecordSymImp(SdPoint *origin, SdPropSymImp *imp, int *logSection, int *logNumber, SdRect *over, QString *prefix, SdPropText *identProp, SdPoint *identOrigin, SdPoint *identPos, SdRect *identRect) :
  SdUndoRecord(),
  mOriginSrc(origin),
  mOriginValue(*origin),
  mPropSrc(imp),
  mLogSectionSrc(logSection),
  mLogSectionValue(*logSection),
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




void SdUndoRecordSymImp::undo()
  {
  mOriginValue.swap( mOriginSrc );
  mPropSrc->swapState( &mProp );

  int tmp = *mLogSectionSrc;
  *mLogNumberSrc = mLogSectionValue;
  mLogSectionValue = tmp;

  tmp = *mLogNumberSrc;
  *mLogNumberSrc = mLogNumberValue;
  mLogNumberValue = tmp;

  mOver = mOverSrc->swap( mOver );
  mPrefixSrc->swap( mPrefix );
  mIdentPropSrc->swapState( &mIdentProp );
  mIdentOrigin.swap( mIdentOriginSrc );
  mIdentPos.swap( mIdentPosSrc );
  mIdentRect = mIdentRectSrc->swap( mIdentRect );
  }
