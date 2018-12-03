/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Undo record for full string-string map
*/
#include "SdUndoRecordStringMap.h"

SdUndoRecordStringMap::SdUndoRecordStringMap(SdStringMap *assoc) :
  mSrc(assoc),
  mAssoc(*assoc)
  {

  }


void SdUndoRecordStringMap::undo()
  {
  mSrc->swap( mAssoc );
  }
