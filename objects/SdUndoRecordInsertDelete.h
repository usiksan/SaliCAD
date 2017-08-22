/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Undo record for inserted or deleted object
*/
#ifndef SDUNDORECORDINSERT_H
#define SDUNDORECORDINSERT_H

#include "SdUndoRecord.h"
#include "SdObject.h"
#include "SdContainer.h"

class SdUndoRecordInsertDelete : public SdUndoRecord
  {
    SdContainer *mContainer;
    SdObject    *mObject;
    bool         mInsert;
  public:
    SdUndoRecordInsertDelete( SdContainer *container, SdObject *object, bool insert );

    // SdUndoRecord interface
  public:
    //Undo elementary operation.
    virtual void undo() override;

    //Redo elementary operation.
    virtual void redo() override;
  };

#endif // SDUNDORECORDINSERT_H
