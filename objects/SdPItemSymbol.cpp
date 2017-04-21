/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/

#include "SdPItemSymbol.h"

SdPItemSymbol::SdPItemSymbol()
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


void SdPItemSymbol::cloneFrom( const SdObject *src )
  {
  SdContainer::cloneFrom( src );
  }
