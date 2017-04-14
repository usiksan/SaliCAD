/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/
#ifndef SDUNDORECORDINSERT_H
#define SDUNDORECORDINSERT_H

#include "SdUndoRecord.h"
#include "SdObject.h"
#include "SdContainer.h"

class SdUndoRecordInsert : public SdUndoRecord
  {
    SdContainer *mContainer;
    SdObject    *mObject;
  public:
    SdUndoRecordInsert( SdContainer *container, SdObject *object );

    // SdUndoRecord interface
  public:
    //Undo elementary operation. Return true on last undo (first operation in undo block)
    virtual void undo() override;

    //Redo elementary operation. Return true on first redo
    virtual void redo() override;
  };

#endif // SDUNDORECORDINSERT_H
