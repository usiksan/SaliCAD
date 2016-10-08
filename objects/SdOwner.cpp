/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/

#include "SdOwner.h"

SdOwner::SdOwner()
  {

  }

void SdOwner::readObject(SdObjectMap *map, const QJsonObject obj)
  {
  SdObject::readObject( map, obj );
  }


void SdOwner::writeObject(QJsonObject &obj) const
  {
  Q_UNUSED(obj)
  }
