/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Undo record for part pin properties and point.
*/
#ifndef SDUNDORECORDPROPPARTPIN_H
#define SDUNDORECORDPROPPARTPIN_H

#include "SdUndoRecord.h"
#include "SdPropPartPin.h"
#include "SdPoint.h"



class SdUndoRecordPropPartPin : public SdUndoRecord
  {
    SdPropPartPin      *mSrcProp;   //Source of property
    SdPropPartPinState  mProp;      //Saved value of property
    SdPoint            *mSrcOrigin; //Source of origin
    SdPoint             mOrigin;    //Saved value of origin
  public:
    SdUndoRecordPropPartPin(SdPropPartPin *prp, SdPoint *org);

    // SdUndoRecord interface
  public:
    virtual void undo() override;
  };

#endif // SDUNDORECORDPROPPARTPIN_H
