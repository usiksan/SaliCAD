/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Undo record for line properties, one point and int.
*/
#ifndef SDUNDORECORDPROPLINEPOINTINT_H
#define SDUNDORECORDPROPLINEPOINTINT_H

#include "SdUndoRecord.h"
#include "SdPropLine.h"
#include "SdPoint.h"

class SdUndoRecordPropLinePointInt : public SdUndoRecord
  {
    SdPropLine *mProp;
    SdPropLine  mPropState;
    SdPoint    *mSrc1;
    SdPoint     mPoint1;
    int        *mSrc2;
    int         mValue2;
  public:
    SdUndoRecordPropLinePointInt( SdPropLine *prop, SdPoint *p1, int *val );

    //Undo elementary operation.
    virtual void    undo() override;
  };

#endif // SDUNDORECORDPROPLINEPOINTINT_H
