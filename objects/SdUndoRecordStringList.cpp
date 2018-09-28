/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Undo string list with one int
*/
#include "SdUndoRecordStringList.h"
#include "SdUtil.h"

SdUndoRecordStringList::SdUndoRecordStringList( qint32 *val, QStringList *list ) :
  SdUndoRecord(),
  mVal(*val),
  mValSrc(val),
  mList( *list ),
  mListSrc(list)
  {

  }


void SdUndoRecordStringList::undo()
  {
  SdUtil::swapInt( mVal, mValSrc );
  mListSrc->swap( mList );
  }
