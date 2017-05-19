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
#ifndef SDUNDORECORDPLATEPOINTER_H
#define SDUNDORECORDPLATEPOINTER_H

#include "SdUndoRecord.h"

class SdPItemPlate;
typedef SdPItemPlate *SdPItemPlatePtr;


class SdUndoRecordPlatePointer : public SdUndoRecord
  {
    SdPItemPlatePtr  mValue;
    SdPItemPlatePtr *mSrc;
  public:
    SdUndoRecordPlatePointer( SdPItemPlatePtr *src );

    // SdUndoRecord interface
  public:
    virtual void undo() override;
  };

#endif // SDUNDORECORDPLATEPOINTER_H
