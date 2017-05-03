/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Undo record for one point.
*/
#ifndef SDUNDORECORDPOINT_H
#define SDUNDORECORDPOINT_H

#include "SdUndoRecord.h"
#include "SdPoint.h"

class SdUndoRecordPoint : public SdUndoRecord
  {
    SdPoint *mSource; //Pointer to source point
    SdPoint  mPoint;  //Stored value
  public:
    SdUndoRecordPoint( SdPoint *src );

    // SdUndoRecord interface
  public:
    virtual void undo() override;
  };

#endif // SDUNDORECORDPOINT_H
