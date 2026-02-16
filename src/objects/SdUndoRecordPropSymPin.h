/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Undo record for symbol pin properties and point.
*/
#ifndef SDUNDORECORDPROPSYMPIN_H
#define SDUNDORECORDPROPSYMPIN_H

#include "SdUndoRecord.h"
#include "SdPropSymPin.h"
#include "SdPoint.h"


class SdUndoRecordPropSymPin : public SdUndoRecord
  {
    SdPropSymPin *mSrcProp;   //Source of property
    SdPropSymPin  mProp;      //Saved value of property
    SdPoint      *mSrcOrigin; //Source of origin
    SdPoint       mOrigin;    //Saved value of origin
  public:
    SdUndoRecordPropSymPin( SdPropSymPin *prp, SdPoint *org );

    // SdUndoRecord interface
  public:
    virtual void undo() override;
  };

#endif // SDUNDORECORDPROPSYMPIN_H
