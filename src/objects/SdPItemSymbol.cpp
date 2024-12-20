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


SdPItemSymbol::SdPItemSymbol() :
  SdPItemWithPart()
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
  if( !isEditEnable() ) {
    if( mThereNewer )
      return QStringLiteral(":/pic/iconSymbolLockedNew.png");
    return QStringLiteral(":/pic/iconSymbolLocked.png");
    }
  return QStringLiteral(":/pic/iconSymbol.png");
  }




quint64 SdPItemSymbol::getAcceptedObjectsMask() const
  {
  return dctSymbolObjects;
  }





