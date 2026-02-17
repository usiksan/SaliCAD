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

SdUndoRecordPartImp::SdUndoRecordPartImp(SdPoint *origin, SdPropPartImp *imp, int *logNumber, SdRect *over ) :
  SdUndoRecord(),
  mOriginSrc(origin),
  mOriginValue(*origin),
  mPropSrc(imp),
  mProp(*imp),
  mLogNumberSrc(logNumber),
  mLogNumberValue(*logNumber),
  mOverSrc(over),
  mOver(*over)
  {
  }




void SdUndoRecordPartImp::undo()
  {
  mOriginValue.swap( mOriginSrc );
  mPropSrc->swap( mProp );

  SdUtil::swap( mLogNumberValue, mLogNumberSrc );

  mOver = mOverSrc->swap( mOver );
  }
