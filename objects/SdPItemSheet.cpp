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
#include "SdContainerSheetNet.h"
#include "SdGraphWiringWire.h"
#include "SdGraphArea.h"
#include "SdGraphSymImp.h"
#include "SdGraphIdent.h"
#include "SdProject.h"

SdPItemSheet::SdPItemSheet()
  {

  }



//get net by its name
SdContainerSheetNet *SdPItemSheet::netGet(const QString name)
  {
  for( SdObject *ptr : mChildList )
    if( ptr && !ptr->isDeleted() && ptr->getClass() == dctSheetNet ) {
      SdContainerSheetNet *net = dynamic_cast<SdContainerSheetNet*>(ptr);
      if( net && net->getNetName() == name ) return net;
      }
  return nullptr;
  }




//Creates net with desired name or return existing net
SdContainerSheetNet *SdPItemSheet::netCreate(const QString name, SdUndo *undo)
  {
  SdContainerSheetNet *net = netGet( name );
  if( net ) return net;
  net = new SdContainerSheetNet( name );
  insertChild( net, undo );
  return net;
  }




void SdPItemSheet::netWirePlace(SdPoint a, SdPoint b, const QString name, SdUndo *undo)
  {
  forEach( dctSymImp, [a, b, name, undo] (SdObject *obj) -> bool {
    SdGraphSymImp *sym = dynamic_cast<SdGraphSymImp*>(obj);
    Q_ASSERT( sym != nullptr );
    sym->netWirePlace( a, b, name, undo );
    return true;
    });
  }




void SdPItemSheet::insertWire(const QString name, SdGraphWiringWire *wire, SdUndo *undo)
  {
  SdContainerSheetNet *net = netCreate( name, undo );
  Q_ASSERT( net != nullptr );
  net->insertChild( wire, undo );
  }



bool SdPItemSheet::getNetFromPoint(SdPoint p, QString &dest)
  {
  SdContainerSheetNet *net = nullptr;
  forEach( dctSheetNet, [&net,p] (SdObject *obj) -> bool {
    net = dynamic_cast<SdContainerSheetNet*>(obj);
    Q_ASSERT( net != nullptr );
    bool on = false;
    net->forEach( dctWire, [&on,p] (SdObject *obj) -> bool {
      SdGraphWiringWire *wire = dynamic_cast<SdGraphWiringWire*>(obj);
      Q_ASSERT( wire != nullptr );
      on = wire->isPointOnSection( p );
      return !on;
      } );
    if( on ) return false;
    net = nullptr;
    return true;
    });
  if( net ) {
    dest = net->getNetName();
    return true;
    }
  return false;
  }




SdPItemPlate *SdPItemSheet::getPlate(SdPoint p)
  {
  SdPItemPlate *plate = nullptr;
  forEach( dctArea, [&plate,p] (SdObject *obj) -> bool {
    SdGraphArea *area = dynamic_cast<SdGraphArea*>( obj );
    Q_ASSERT( area != nullptr );
    if( area->isPointInside(p) ) {
      plate = area->getPlate();
      return false;
      }
    return true;
    });
  if( plate == nullptr )
    return getProject()->getDefaultPlate();
  return plate;
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





SdGraphIdent *SdPItemSheet::createIdent()
  {
  SdGraphIdent *ident = new SdGraphIdent( SdPoint(), QStringLiteral("Id"), SdRect(), sdGlobalProp->mSymIdentProp );
  SdProject *prj = getProject();
  Q_ASSERT( prj != nullptr );
  insertChild( ident, prj->getUndo() );
  return ident;
  }

