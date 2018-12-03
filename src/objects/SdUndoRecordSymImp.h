/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Undo record for symbol implementation in sheet
*/
#ifndef SDUNDORECORDSYMIMP_H
#define SDUNDORECORDSYMIMP_H

#include "SdUndoRecord.h"
#include "SdPoint.h"
#include "SdRect.h"
#include "SdPropSymImp.h"
#include "SdPropText.h"

class SdUndoRecordSymImp : public SdUndoRecord
  {
    SdPoint           *mOriginSrc;
    SdPoint            mOriginValue;
    SdPropSymImp      *mPropSrc;
    SdPropSymImpState  mProp;
    int               *mLogSectionSrc;
    int                mLogSectionValue;
    int               *mLogNumberSrc;
    int                mLogNumberValue;
    SdRect            *mOverSrc;
    SdRect             mOver;
  public:
    SdUndoRecordSymImp( SdPoint *origin, SdPropSymImp *imp, int *logSection, int *logNumber, SdRect *over );

    // SdUndoRecord interface
  public:
    virtual void undo() override;
  };

#endif // SDUNDORECORDSYMIMP_H
