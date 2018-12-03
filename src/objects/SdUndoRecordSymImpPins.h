/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Undo record for implement symbol all pins.
*/
#ifndef SDUNDORECORDSYMIMPPINS_H
#define SDUNDORECORDSYMIMPPINS_H

#include "SdUndo.h"
#include "SdUndoRecord.h"
#include "SdSymImpPin.h"

class SdUndoRecordSymImpPins : public SdUndoRecord
  {
    SdSymImpPinTable *mSource;
    SdSymImpPinTable  mPins;
  public:
    SdUndoRecordSymImpPins( SdSymImpPinTable *pins );

    // SdUndoRecord interface
  public:
    virtual void undo() override;
  };

#endif // SDUNDORECORDSYMIMPPINS_H
