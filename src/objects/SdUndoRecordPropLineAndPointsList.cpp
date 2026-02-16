/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Undo record for line properties and point list.
*/
#include "SdUndoRecordPropLineAndPointsList.h"

SdUndoRecordPropLineAndPointsList::SdUndoRecordPropLineAndPointsList(SdPropLine *prp, SdPointList *src) :
  SdUndoRecord(),
  mProp(prp),
  mPropState(*prop),
  mSrc(src),
  mPointList(*src)
  {
  }




void SdUndoRecordPropLineAndPointsList::undo()
  {
  mProp->swap( mPropState );
  mPointList.swap( *mSrc );
  }


