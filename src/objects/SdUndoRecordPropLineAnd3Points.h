/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Undo record for line properties and 3 points.
*/
#ifndef SDUNDORECORDPROPLINEAND2POINTS_H
#define SDUNDORECORDPROPLINEAND2POINTS_H

#include "SdUndoRecord.h"
#include "SdPropLine.h"
#include "SdPoint.h"

class SdUndoRecordPropLineAnd3Points : public SdUndoRecord
  {
    SdPropLine *mProp;
    SdPropLine  mPropState;
    SdPoint    *mSrc1;
    SdPoint     mPoint1;
    SdPoint    *mSrc2;
    SdPoint     mPoint2;
    SdPoint    *mSrc3;
    SdPoint     mPoint3;
  public:
    SdUndoRecordPropLineAnd3Points( SdPropLine *prop, SdPoint *p1, SdPoint *p2, SdPoint *p3 = nullptr );

    //Undo elementary operation.
    virtual void    undo() override;
  };

#endif // SDUNDORECORDPROPLINEAND2POINTS_H
