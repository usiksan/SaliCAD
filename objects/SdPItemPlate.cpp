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
#include "SdGraphIdent.h"
#include "SdGraphPartImp.h"
#include "SdProject.h"

SdPItemPlate::SdPItemPlate()
  {

  }




SdGraphPartImp *SdPItemPlate::allocPartImp(int *section, SdPItemPart *part, SdPItemSymbol *comp, SdPItemSymbol *sym, SdUndo *undo )
  {
  //At first, scan all part implements and find match
  SdGraphPartImp *res = nullptr;
  forEach( dctPartImp, [&] (SdObject *obj) -> bool {
    res = dynamic_cast<SdGraphPartImp*>( obj );
    Q_ASSERT( res != nullptr );
    if( res->isSectionFree( section, part, comp, sym ) )
      return false;
    res = nullptr;
    return true;
    });
  //If part implement found then return it
  if( res )
    return res;

  //Create new part implement
  res = new SdGraphPartImp( SdPoint(), part, comp );
  insertChild( res, undo );

  Q_ASSERT( res->isSectionFree( section, part, comp, sym ) );
  return res;
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




SdGraphIdent *SdPItemPlate::createIdent()
  {
  SdGraphIdent *ident = new SdGraphIdent( SdPoint(), QStringLiteral("Id"), SdRect(), sdGlobalProp->mPartIdentProp );
  SdProject *prj = getProject();
  Q_ASSERT( prj != nullptr );
  insertChild( ident, prj->getUndo() );
  return ident;
  }

