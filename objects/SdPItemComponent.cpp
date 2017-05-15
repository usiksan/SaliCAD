/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/
#include "SdPItemComponent.h"

SdPItemComponent::SdPItemComponent() :
  SdPItemSymbol()
  {

  }





QString SdPItemComponent::getType() const
  {
  return QString(SD_TYPE_COMPONENT);
  }




quint64 SdPItemComponent::getClass() const
  {
  return dctComponent;
  }




QString SdPItemComponent::getIconName() const
  {
  return QString(":/pic/iconProject.png");
  }
