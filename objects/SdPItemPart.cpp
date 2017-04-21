/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/
#include "SdPItemPart.h"

SdPItemPart::SdPItemPart()
  {

  }


QString SdPItemPart::getType() const
  {
  return QString( SD_TYPE_PART );
  }




quint64 SdPItemPart::getClass() const
  {
  return dctPart;
  }




QString SdPItemPart::getIconName() const
  {
  return QString(":/pic/iconPart.png");
  }
