/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Undo record for wire in schematic sheets
*/
#ifndef SDUNDORECORDWIRE_H
#define SDUNDORECORDWIRE_H

#include "SdUndoRecord.h"
#include "SdPropLine.h"
#include "SdPoint.h"


class SdUndoRecordWire : public SdUndoRecord
  {
    SdPropLine      *mProp;
    SdPropLineState  mPropState;
    SdPoint         *mSrc1;
    SdPoint          mPoint1;
    SdPoint         *mSrc2;
    SdPoint          mPoint2;
    bool            *mDotSrc1;
    bool             mDot1;
    bool            *mDotSrc2;
    bool             mDot2;
  public:
    SdUndoRecordWire( SdPropLine *prop, SdPoint *p1, SdPoint *p2, bool *dot1, bool *dot2 );

    //Undo elementary operation.
    virtual void    undo() override;
  };

#endif // SDUNDORECORDWIRE_H
