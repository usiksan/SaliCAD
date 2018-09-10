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
  mLogNumberSrc(logNumber),
  mLogNumberValue(*logNumber),
  mOverSrc(over),
  mOver(*over)
  {
  imp->saveState( &mProp );
  }




void SdUndoRecordPartImp::undo()
  {
  mOriginValue.swap( mOriginSrc );
  mPropSrc->swapState( &mProp );

  SdUtil::swapInt( mLogNumberValue, mLogNumberSrc );

  mOver = mOverSrc->swap( mOver );
  }
