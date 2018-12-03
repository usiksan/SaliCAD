/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Undo record for implement symbol pin properties and point.
*/
#ifndef SDUNDORECORDSYMIMPPIN_H
#define SDUNDORECORDSYMIMPPIN_H

#include "SdUndoRecord.h"
#include "SdSymImpPin.h"

class SdGraphSymImp;
class SdGraphPartImp;

class SdUndoRecordSymImpPin : public SdUndoRecord
  {
    SdGraphSymImp  *mSymImp;
    QString         mPinName;   //Pin name in symbol
    SdSymImpPin     mPinState;  //Symbol implement pin state
  public:
    SdUndoRecordSymImpPin( SdGraphSymImp *imp, QString pinName );

    // SdUndoRecord interface
  public:
    virtual void undo() override;
  };

#endif // SDUNDORECORDSYMIMPPIN_H
