/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Undo record for via
*/
#ifndef SDUNDORECORDVIA_H
#define SDUNDORECORDVIA_H

#include "SdUndoRecord.h"
#include "SdPoint.h"
#include "SdPropString.h"


class SdUndoRecordVia : public SdUndoRecord
  {
    QString       mPadType;
    SdPropString *mSrcPadType;
    SdPoint       mPosition;
    SdPoint      *mSrcPosition;
  public:
    SdUndoRecordVia( SdPropString *srcPadType, SdPoint *srcPosition );

    // SdUndoRecord interface
  public:
    virtual void undo() override;
  };

#endif // SDUNDORECORDVIA_H
