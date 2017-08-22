/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Undo record for deleted object
*/
#ifndef SDUNDORECORDDELETE_H
#define SDUNDORECORDDELETE_H

#include "SdUndoRecord.h"
#include "SdObject.h"
#include "SdContainer.h"

class SdUndoRecordDelete : public SdUndoRecord
  {
    SdContainer *mContainer;
    SdObject    *mObject;
  public:
    SdUndoRecordDelete( SdContainer *container, SdObject *object );

    // SdUndoRecord interface
  public:
    //Undo elementary operation.
    virtual void undo() override;

    //Redo elementary operation.
    virtual void redo() override;

  };

#endif // SDUNDORECORDDELETE_H
