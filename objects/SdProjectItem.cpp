/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/

#include "SdProjectItem.h"

SdProjectItem::SdProjectItem()
  {

  }



void SdProjectItem::writeObject(QJsonObject &obj)
  {
  SdObject::writeObject(obj);
  obj.insert( QString(SDKO_TITLE), mTitle );
  }

void SdProjectItem::readObject(SdObjectMap *map, const QJsonObject obj)
  {
  SdObject::readObject( map, obj );
  mTitle = obj.value( QString(SDKO_TITLE) );
  }
