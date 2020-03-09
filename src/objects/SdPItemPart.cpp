/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Graphics representation part of component
*/
#include "SdPItemPart.h"
#include "SdGraphIdent.h"
#include "SdProject.h"

SdPItemPart::SdPItemPart() :
  SdProjectItem()
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
  if( !isEditEnable() ) {
    if( mThereNewer )
      return QStringLiteral(":/pic/iconPartLockedNew.png");
    return QStringLiteral(":/pic/iconPartLocked.png");
    }
  return QStringLiteral(":/pic/iconPart.png");
  }




quint64 SdPItemPart::getAcceptedObjectsMask() const
  {
  return dctPartObjects;
  }




