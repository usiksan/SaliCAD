/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Undo record for one pin name-number pair
*/
#include "SdUndoRecordStringMapItem.h"


SdUndoRecordStringMapItem::SdUndoRecordStringMapItem(SdStringMap *map, const QString key) :
  mStringMap(map),
  mKey(key)
  {
  mValue = mStringMap->value( key );
  }


void SdUndoRecordStringMapItem::undo()
  {
  QString tmp = mStringMap->value( mKey );
  mStringMap->insert( mKey, mValue );
  mValue = tmp;
  }
