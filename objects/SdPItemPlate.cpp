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
#include "SdPItemPart.h"
#include "SdProject.h"
#include "SdContext.h"
#include "SdEnvir.h"
#include <QDebug>

SdPItemPlate::SdPItemPlate() :
  SdProjectItem(),
  mRatNetDirty(true)
  {

  }






SdGraphPartImp *SdPItemPlate::allocPartImp(int *section, SdPItemPart *part, SdPItemSymbol *comp, SdPItemSymbol *sym, SdUndo *undo )
  {
  if( part == nullptr )
    return nullptr;
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

  //Calculate insert position for part implementation
  if( mPartRow.isEmpty() || mPartRow.isNull() || mPartRow.width() > 100000 ) {
    //Create new row
    mPartRow = getOverRect();
    if( mPartRow.isEmpty() || mPartRow.isNull() )
      mPartRow.set( SdPoint(0,0), SdPoint(2500,2500) );
    mPartRow.setWidth(2500);
    qDebug() << "row start" << mPartRow;
    }
  SdRect over = part->getOverRect();
  SdPoint p( mPartRow.getTopRight() );
  qDebug() << p;
  p.ry() = (p.y() + over.height()) / 2500 * 2500;
  p.rx() = (p.x() + 2500) / 2500 * 2500;
  mPartRow.setRight( p.x() + over.width() );
  qDebug() << Q_FUNC_INFO << mPartRow << p << over.height();

  //Create new part implement
  res = new SdGraphPartImp( p, &(sdGlobalProp->mPartImpProp), part, comp );
  insertChild( res, undo );

  Q_ASSERT( res->isSectionFree( section, part, comp, sym ) );
  return res;
  }




//Set flag to update rat net
void SdPItemPlate::setDirtyRatNet()
  {
  mRatNetDirty = true;
  }



void SdPItemPlate::drawRatNet(SdContext *dc)
  {
  //Build rat net if neccessary
  if( mRatNetDirty )
    buildRatNet();
  //Draw rat net pairs
  dc->setPen( 0, sdEnvir->getSysColor(scRatNet), dltSolid );
  for( const SdRatNetPair &pair : mRatNet )
    dc->line( pair.a, pair.b );
  }




//Build rat net
void SdPItemPlate::buildRatNet()
  {
  //Cleat previous rat net
  mRatNet.clear();

  //Create net list for build rat net
  SdPlateNetList netList;

  //Accum points for nets
  forEach( dctAll, [&netList] (SdObject *obj) -> bool {
    SdGraphTraced *traced = dynamic_cast<SdGraphTraced*>(obj);
    if( traced )
      traced->accumNetPoints( netList );
    return true;
    });

  //Build rat net
  netList.buildRatNet( &mRatNet );

  mRatNetDirty = false;
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




quint64 SdPItemPlate::getAcceptedObjectsMask() const
  {
  return dctPlateObjects;
  }








void SdPItemPlate::insertObjects(SdPoint offset, SdSelector *sel, SdUndo *undo, SdWEditorGraph *editor, SdSelector *dest, bool next)
  {
  //TODO D031 insert objects into plate
  }



SdProjectItem *SdPItemPlate::setEditEnable(bool edit)
  {
  mEditEnable = edit;
  return this;
  }







void SdPItemPlate::writeObject(QJsonObject &obj) const
  {
  SdProjectItem::writeObject( obj );
  mPartRow.write( QStringLiteral("PartRow"), obj );

  //Write pad assotiation
  QMapIterator<QString,SdPItemPart*> it( mPadAssotiation );
  QJsonObject assotiation;
  while( it.hasNext() ) {
    SdPItemPart *part = it.next().value();
    writePtr( part, it.key(), assotiation );
    }
  obj.insert( QStringLiteral("PadAssotiation"), assotiation );
  }




void SdPItemPlate::readObject(SdObjectMap *map, const QJsonObject obj)
  {
  SdProjectItem::readObject( map, obj );
  mPartRow.read( QStringLiteral("PartRow"), obj );
  //Read pad assotiation
  mPadAssotiation.clear();
  QJsonObject assotiation = obj.value( QStringLiteral("PadAssotiation") ).toObject();
  for( QJsonObject::const_iterator it = assotiation.constBegin(); it != assotiation.constEnd(); it++ ) {
    mPadAssotiation.insert( it.key(), dynamic_cast<SdPItemPart*>(readPtr( map, it.value().toObject() ))  );
    }
  }




bool SdPItemPlate::isUsed(SdObject *obj) const
  {
  //Test obj in all child objects
  if( SdContainer::isUsed( obj ) )
    return true;

  //Test obj in pad assotiation
  QMapIterator<QString,SdPItemPart*> it( mPadAssotiation );
  while( it.hasNext() ) {
    if( it.next().value() == obj )
      return true;
    }

  return false;
  }

