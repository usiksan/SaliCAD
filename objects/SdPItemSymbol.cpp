/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Graphical symbol presentation
*/

#include "SdPItemSymbol.h"
#include "SdSection.h"
#include "SdPartVariant.h"
#include "SdObjectFactory.h"
#include "SdGraphIdent.h"
#include "SdProject.h"

SdPItemSymbol::SdPItemSymbol() :
  SdProjectItem()
  {

  }




QString SdPItemSymbol::getType() const
  {
  return QStringLiteral(SD_TYPE_SYMBOL);
  }




quint64 SdPItemSymbol::getClass() const
  {
  return dctSymbol;
  }




QString SdPItemSymbol::getIconName() const
  {
  return QString(":/pic/iconSymbol.png");
  }




quint64 SdPItemSymbol::getAcceptedObjectsMask() const
  {
  return dctSymbolObjects;
  }





void SdPItemSymbol::cloneFrom( const SdObject *src )
  {
  SdProjectItem::cloneFrom( src );
  }

