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
#include "SdGraphIdent.h"
#include "SdProject.h"

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




quint64 SdPItemPart::getAcceptedObjectsMask() const
  {
  return dctPartObjects;
  }



SdGraphIdent *SdPItemPart::createIdent()
  {
  SdGraphIdent *ident = new SdGraphIdent( SdPoint(), QStringLiteral("Id"), SdRect(), sdGlobalProp->mPartIdentProp );
  SdProject *prj = getProject();
  Q_ASSERT( prj != nullptr );
  insertChild( ident, prj->getUndo() );
  return ident;
  }



