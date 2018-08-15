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
#include "SdGraphNetWire.h"
#include "SdGraphArea.h"
#include "SdGraphSymImp.h"
#include "SdGraphIdent.h"
#include "SdProject.h"

SdPItemSheet::SdPItemSheet() :
  SdProjectItem()
  {

  }



//get net by its name
bool SdPItemSheet::isNetPresent(const QString name)
  {
  bool present = false;
  //Find net element with desired name
  forEach( dctNetName | dctNetWire, [&present,name] (SdObject *obj) -> bool {
    SdGraphNet *net = dynamic_cast<SdGraphNet*>(obj);
    if( net != nullptr && net->getNetName() == name ) {
      //Net found, break subsequent repetition
      present = true;
      return false;
      }
    return true;
    } );
  return present;
  }






//Rename net. Both simple rename and union two nets
void SdPItemSheet::netRename(const QString oldName, const QString newName, SdUndo *undo)
  {
  forEach( dctNetWire | dctNetName, [oldName, newName, undo] (SdObject *obj) -> bool {
    SdGraphNet *net = dynamic_cast<SdGraphNet*>(obj);
    if( net != nullptr && net->getNetName() == oldName )
      net->setNetName( newName, undo );
    return true;
    });
  }




//Information about wire segment moving to make connection to pin
void SdPItemSheet::netWirePlace(SdGraphNetWire *wire, SdUndo *undo)
  {
  forEach( dctSymImp, [wire, undo] (SdObject *obj) -> bool {
    SdGraphSymImp *sym = dynamic_cast<SdGraphSymImp*>(obj);
    Q_ASSERT( sym != nullptr );
    sym->netWirePlace( wire, undo );
    return true;
    });
  }



//Information about wire segment delete to remove connection from pin
void SdPItemSheet::netWireDelete( SdGraphNetWire *wire, SdUndo *undo)
  {
  forEach( dctSymImp, [ wire, undo] (SdObject *obj) -> bool {
    SdGraphSymImp *sym = dynamic_cast<SdGraphSymImp*>(obj);
    Q_ASSERT( sym != nullptr );
    sym->netWireDelete( wire, undo );
    return true;
    });
  }






bool SdPItemSheet::getNetFromPoint(SdPoint p, QString &dest)
  {
  dest.clear();
  forEach( dctNetWire, [&dest,p] (SdObject *obj) -> bool {
    SdGraphNetWire *netWire = dynamic_cast<SdGraphNetWire*>(obj);
    Q_ASSERT( netWire != nullptr );
    if( netWire->isPointOnSection(p) ) {
      dest = netWire->getNetName();
      return false;
      }
    return true;
    });
  return !dest.isEmpty();
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





//Accumulate to selector element linked with point and net name
void SdPItemSheet::accumLinked(SdPoint a, SdPoint b, const QString netName, SdSelector *selector, SdUndo *undo)
  {
  forEach( dctNetWire | dctSymImp, [a,b,netName,selector,undo] (SdObject *obj) -> bool {
    SdGraphNetWire *wire = dynamic_cast<SdGraphNetWire*>(obj);
    if( wire != nullptr ) {
      if( wire->getNetName() == netName )
        wire->accumLinked( a, b, selector, undo );
      }
    else {
      SdGraphSymImp *sym = dynamic_cast<SdGraphSymImp*>(obj);
      if( sym != nullptr )
        sym->accumLinked( a, b, netName, selector );
      }
    return true;
    } );
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




quint64 SdPItemSheet::getAcceptedObjectsMask() const
  {
  return dctSheetObjects;
  }




SdGraph *SdPItemSheet::insertCopyObject(const SdGraph *obj, SdPoint offset, SdUndo *undo, SdWEditorGraph *editor, bool next)
  {
  Q_UNUSED(editor)
  if( obj != nullptr && (obj->getClass() & getAcceptedObjectsMask()) ) {
    SdGraph *cp = dynamic_cast<SdGraph*>( next ? obj->copyNext() : obj->copy() );
    Q_ASSERT( cp != nullptr );
    cp->select( nullptr );
    cp->move( offset );
    //For nets we perform substitute new net names for default named nets
    if( cp->getClass() & (dctNetName | dctNetWire) ) {
      SdGraphNet *net = dynamic_cast<SdGraphNet*>(cp);
      Q_ASSERT( net != nullptr );
      //Test if this net with default name
      if( net->getNetName().startsWith(QString(defNetNamePrefix)) ) {
        //This net with default name
        //Test if this net name already in map
        if( !mNetCopyMap.contains(net->getNetName()) )
          //Net name not registered yet - register
          mNetCopyMap.insert( net->getNetName(), getProject()->getUnusedNetName() );
        //This name already registered, substitute
        net->setNetName( mNetCopyMap.value(net->getNetName()), nullptr );
        }
      }
    insertChild( cp, undo );
    return cp;
    }
  return nullptr;
  }





void SdPItemSheet::insertObjects(SdPoint offset, SdSelector *sour, SdUndo *undo, SdWEditorGraph *editor, SdSelector *dest, bool next)
  {
  //Reset net map for new copy loop
  mNetCopyMap.clear();
  //Exec copy loop
  SdProjectItem::insertObjects( offset, sour, undo, editor, dest, next );
  }






void SdPItemSheet::writeObject(QJsonObject &obj) const
  {
  SdProjectItem::writeObject( obj );
  obj.insert( QStringLiteral("index"), mSheetIndex );
  }




void SdPItemSheet::readObject(SdObjectMap *map, const QJsonObject obj)
  {
  SdProjectItem::readObject( map, obj );
  mSheetIndex = obj.value( QStringLiteral("index") ).toInt();
  }

