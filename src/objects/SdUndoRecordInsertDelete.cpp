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
#include "SdUndoRecordInsertDelete.h"

SdUndoRecordInsertDelete::SdUndoRecordInsertDelete(SdContainer *container, SdObject *object, bool insert) :
  SdUndoRecord(),
  mContainer(container),
  mObject(object),
  mInsert(insert)
  {

  }


void SdUndoRecordInsertDelete::undo()
  {
  if( mInsert )
    mContainer->undoInsertChild( mObject );
  else
    mContainer->undoDeleteChild( mObject );
  }




void SdUndoRecordInsertDelete::redo()
  {
  if( mInsert )
    mContainer->redoInsertChild( mObject );
  else
    mContainer->redoDeleteChild( mObject );
  }
