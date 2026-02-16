/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Undo record for line properties, rect and 2 ints.
*/
#ifndef SDUNDORECORDPROPLINERECT2INT_H
#define SDUNDORECORDPROPLINERECT2INT_H

#include "SdUndoRecord.h"
#include "SdPropLine.h"
#include "SdRect.h"


class SdUndoRecordPropLineRect2Int : public SdUndoRecord
  {
    SdPropLine *mProp;
    SdPropLine  mPropState;
    SdRect     *mRectSrc;
    SdRect      mRect;
    int        *mValSrc1;
    int         mVal1;
    int        *mValSrc2;
    int         mVal2;
  public:
    SdUndoRecordPropLineRect2Int( SdPropLine *prp, SdRect *r, int *v1, int *v2 );

    // SdUndoRecord interface
  public:
    virtual void undo() override;
  };

#endif // SDUNDORECORDPROPLINERECT2INT_H
