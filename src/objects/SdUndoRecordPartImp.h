/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Undo record for part implementation in plate
*/
#ifndef SDUNDORECORDPARTIMP_H
#define SDUNDORECORDPARTIMP_H

#include "SdUndoRecord.h"
#include "SdPoint.h"
#include "SdRect.h"
#include "SdPropPartImp.h"
#include "SdPropText.h"


class SdUndoRecordPartImp : public SdUndoRecord
  {
    SdPoint       *mOriginSrc;
    SdPoint        mOriginValue;
    SdPropPartImp *mPropSrc;
    SdPropPartImp  mProp;
    int           *mLogNumberSrc;
    int            mLogNumberValue;
    SdRect        *mOverSrc;
    SdRect         mOver;
  public:
    SdUndoRecordPartImp( SdPoint *origin, SdPropPartImp *imp, int *logNumber, SdRect *over );

    // SdUndoRecord interface
  public:
    virtual void undo() override;
  };

#endif // SDUNDORECORDPARTIMP_H
