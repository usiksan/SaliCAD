/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/

#include "SdSymbol.h"

SdSymbol::SdSymbol()
  {

  }



QString SdSymbol::getType() const
  {
  return QStringLiteral(SD_TYPE_SYMBOL);
  }

quint64 SdSymbol::getClass() const
  {
  return dctSymbol;
  }


QString SdSymbol::getIconName() const
  {
  return QString(":/pic/iconSymbol.png");
  }


void SdSymbol::cloneFrom( const SdObject *src )
  {
  SdContainer::cloneFrom( src );
  }
