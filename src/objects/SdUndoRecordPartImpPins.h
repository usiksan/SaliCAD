/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Undo record for implement part all pins.
*/
#ifndef SDUNDORECORDPARTIMPPINS_H
#define SDUNDORECORDPARTIMPPINS_H

#include "SdUndoRecord.h"
#include "SdGraphPartImp.h"

class SdUndoRecordPartImpPins : public SdUndoRecord
  {
    SdPartImpPinTable *mSource;
    SdPartImpPinTable  mPins;
  public:
    SdUndoRecordPartImpPins( SdPartImpPinTable *table );

    // SdUndoRecord interface
  public:
    virtual void undo() override;
  };

#endif // SDUNDORECORDPARTIMPPINS_H
