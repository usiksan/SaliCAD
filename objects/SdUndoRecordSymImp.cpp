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
#include "SdUtil.h"


SdUndoRecordSymImp::SdUndoRecordSymImp(SdPoint *origin, SdPropSymImp *imp, int *logSection, int *logNumber, SdRect *over) :
  SdUndoRecord(),
  mOriginSrc(origin),
  mOriginValue(*origin),
  mPropSrc(imp),
  mLogSectionSrc(logSection),
  mLogSectionValue(*logSection),
  mLogNumberSrc(logNumber),
  mLogNumberValue(*logNumber),
  mOverSrc(over),
  mOver(*over)
  {
  imp->saveState( &mProp );
  }




void SdUndoRecordSymImp::undo()
  {
  mOriginValue.swap( mOriginSrc );
  mPropSrc->swapState( &mProp );

  SdUtil::swapInt( mLogSectionValue, mLogSectionSrc );

  SdUtil::swapInt( mLogNumberValue, mLogNumberSrc );

  mOver = mOverSrc->swap( mOver );
  }
