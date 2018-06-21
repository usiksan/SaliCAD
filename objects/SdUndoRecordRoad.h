/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Undo record for roads
*/
#ifndef SDUNDORECORDROAD_H
#define SDUNDORECORDROAD_H

#include "SdUndoRecord.h"
#include "SdPoint.h"
#include "SdPropInt.h"

class SdUndoRecordRoad : public SdUndoRecord
  {
    int        mWidth;
    SdPropInt *mSrcWidth;
    SdPoint    mP1;
    SdPoint   *mSrcP1;
    SdPoint    mP2;
    SdPoint   *mSrcP2;
  public:
    SdUndoRecordRoad( SdPropInt *width, SdPoint *p1, SdPoint *p2 );

    // SdUndoRecord interface
  public:
    virtual void undo() override;
  };

#endif // SDUNDORECORDROAD_H
