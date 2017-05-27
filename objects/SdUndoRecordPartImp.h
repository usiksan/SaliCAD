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
    SdPoint           *mOriginSrc;
    SdPoint            mOriginValue;
    SdPropPartImp     *mPropSrc;
    SdPropPartImpState mProp;
    int               *mLogNumberSrc;
    int                mLogNumberValue;
    SdRect            *mOverSrc;
    SdRect             mOver;
    QString           *mPrefixSrc;
    QString            mPrefix;
    SdPropText        *mIdentPropSrc;
    SdPropTextState    mIdentProp;
    SdPoint           *mIdentOriginSrc;
    SdPoint            mIdentOrigin;
    SdPoint           *mIdentPosSrc;
    SdPoint            mIdentPos;
    SdRect            *mIdentRectSrc;
    SdRect             mIdentRect;
  public:
    SdUndoRecordPartImp( SdPoint *origin, SdPropPartImp *imp, int *logNumber, SdRect *over, QString *prefix, SdPropText *identProp,
                         SdPoint *identOrigin, SdPoint *identPos, SdRect *identRect );

    // SdUndoRecord interface
  public:
    virtual void undo() override;
  };

#endif // SDUNDORECORDPARTIMP_H
