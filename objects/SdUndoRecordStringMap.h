/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Undo record for full string-string map
*/
#ifndef SDUNDORECORDSTRINGMAP_H
#define SDUNDORECORDSTRINGMAP_H

#include "SdUndoRecord.h"
#include "SdStringMap.h"

class SdUndoRecordStringMap : public SdUndoRecord
  {
    SdStringMap *mSrc;   //Source of association
    SdStringMap  mAssoc; //Stored association
  public:
    SdUndoRecordStringMap( SdStringMap *assoc );

    // SdUndoRecord interface
  public:
    virtual void undo() override;
  };

#endif // SDUNDORECORDSTRINGMAP_H
