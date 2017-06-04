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
#include "SdContainerPlateNet.h"

SdPItemPlate::SdPItemPlate()
  {

  }




//get net by its name
SdContainerPlateNet *SdPItemPlate::netGet(const QString name)
  {
  for( SdObject *ptr : mChildList )
    if( ptr && !ptr->isDeleted() && ptr->getClass() == dctPlateNet ) {
      SdContainerPlateNet *net = dynamic_cast<SdContainerPlateNet*>(ptr);
      if( net && net->getNetName() == name ) return net;
      }
  return nullptr;
  }




//Creates net with desired name or return existing net
SdContainerPlateNet *SdPItemPlate::netCreate(const QString name, SdUndo *undo)
  {
  SdContainerPlateNet *net = netGet( name );
  if( net ) return net;
  net = new SdContainerPlateNet( name );
  insertChild( net, undo );
  return net;
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
  res = new SdGraphPartImp( SdPoint(), &(sdGlobalProp->mPartImpProp), part, comp );
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

