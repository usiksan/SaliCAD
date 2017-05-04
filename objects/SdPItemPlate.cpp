/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Graphical PCB layout with component placement
*/
#include "SdPItemPlate.h"

SdPItemPlate::SdPItemPlate()
  {

  }


QString SdPItemPlate::getType() const
  {
  return QStringLiteral( SD_TYPE_PLATE );
  }

quint64 SdPItemPlate::getClass() const
  {
  return dctPlate;
  }

void SdPItemPlate::cloneFrom(const SdObject *src)
  {
  SdProjectItem::cloneFrom( src );
  }

QString SdPItemPlate::getIconName() const
  {
  return QStringLiteral(":/pic/iconPlate.png");
  }
