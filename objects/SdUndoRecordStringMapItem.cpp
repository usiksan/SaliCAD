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
  mPresence = mStringMap->contains(key);
  if( mPresence )
    mValue = mStringMap->value( key );
  }




void SdUndoRecordStringMapItem::undo()
  {
  if( mPresence ) {
    QString tmp = mStringMap->value( mKey );
    mStringMap->insert( mKey, mValue );
    mValue = tmp;
    }
  else {
    mValue = mStringMap->value( mKey );
    mStringMap->remove(mKey);
    }
  }




void SdUndoRecordStringMapItem::redo()
  {
  if( mPresence ) {
    QString tmp = mStringMap->value( mKey );
    mStringMap->insert( mKey, mValue );
    mValue = tmp;
    }
  else mStringMap->insert( mKey, mValue );
  }

