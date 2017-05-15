/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Graphical schematic sheet presentation
*/
#include "SdPItemSheet.h"

SdPItemSheet::SdPItemSheet()
  {

  }



SdPItemPlate *SdPItemSheet::getPlate(SdPoint p)
  {
  //TODO select plate from areas or constructiv
  }



QString SdPItemSheet::getType() const
  {
  return QStringLiteral( SD_TYPE_SHEET );
  }



quint64 SdPItemSheet::getClass() const
  {
  return dctSheet;
  }



void SdPItemSheet::cloneFrom(const SdObject *src)
  {
  SdProjectItem::cloneFrom( src );
  }




QString SdPItemSheet::getIconName() const
  {
  return QStringLiteral(":/pic/iconSheet.png");
  }

