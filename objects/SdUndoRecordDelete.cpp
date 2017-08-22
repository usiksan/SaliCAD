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
#include "SdUndoRecordDelete.h"

SdUndoRecordDelete::SdUndoRecordDelete(SdContainer *container, SdObject *object) :
  SdUndoRecord(),
  mContainer(container),
  mObject(object)
  {

  }



//Undo elementary operation.
void SdUndoRecordDelete::undo()
  {
  mContainer->undoDeleteChild( mObject );
  }




//Redo elementary operation.
void SdUndoRecordDelete::redo()
  {
  mContainer->redoDeleteChild( mObject );
  }



