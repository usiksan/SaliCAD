/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/
#include "SdUndoRecordInsert.h"

SdUndoRecordInsert::SdUndoRecordInsert(SdContainer *container, SdObject *object) :
  SdUndoRecord(),
  mContainer(container),
  mObject(object)
  {

  }


void SdUndoRecordInsert::undo()
  {
  mContainer->undoInsertChild( mObject );
  }

void SdUndoRecordInsert::redo()
  {
  mContainer->redoInsertChild( mObject );
  }
