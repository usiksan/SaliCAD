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



void SdProjectItem::writeObject(QJsonObject &obj) const
  {
  //SdOwner::writeObject( obj );
  obj.insert( QStringLiteral("Title"), mTitle );
  }


void SdProjectItem::readObject(SdObjectMap *map, const QJsonObject obj)
  {
  SdOwner::readObject( map, obj );
  mTitle = obj.value( QStringLiteral("Title") ).toString();
  }
