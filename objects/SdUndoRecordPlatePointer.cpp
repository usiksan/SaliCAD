/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Plate pointer value
*/
#include "SdUndoRecordPlatePointer.h"

SdUndoRecordPlatePointer::SdUndoRecordPlatePointer(SdPItemPlatePtr *src) :
  SdUndoRecord(),
  mValue(*src),
  mSrc(src)
  {

  }


void SdUndoRecordPlatePointer::undo()
  {
  SdPItemPlatePtr tmp = *mSrc;
  mSrc = mValue;
  mValue = tmp;
  }

