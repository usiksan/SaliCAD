/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/
#include "SdUndoRecordProjectItem.h"
#include "SdUtil.h"
#include "SdPulsar.h"

SdUndoRecordProjectItem::SdUndoRecordProjectItem(SdProjectItem *item, QString *title, QString *author, QString *tag, int *timeCreation, bool *editEnable) :
  SdUndoRecord(),
  mItem(item),
  mTitle(*title),
  mTitleSrc(title),
  mAuthor(*author),
  mAuthorSrc(author),
  mTag(*tag),
  mTagSrc(tag),
  mTimeCreation(*timeCreation),
  mTimeCreationSrc(timeCreation),
  mEditEnable(*editEnable),
  mEditEnableSrc(editEnable)
  {

  }



//Undo elementary operation.
void SdUndoRecordProjectItem::undo()
  {
  //Close item editors (if any)
  SdPulsar::sdPulsar->emitCloseEditView( mItem );

  //Undo
  mTitleSrc->swap( mTitle );
  mAuthorSrc->swap( mAuthor );
  mTagSrc->swap( mTag );
  SdUtil::swapInt( mTimeCreation, mTimeCreationSrc );
  SdUtil::swapBool( mEditEnable, mEditEnableSrc );

  }

