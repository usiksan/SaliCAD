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
#include <QDebug>

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
  //TODO draw rat net
  if( mRatNetDirty ) {
    //Cleat previous rat net
    mRatNet.clear();

    //Accum ratNet pairs

    //At first, accum sub nets
    mSubNet.clear();
    mSubNet.append( 0 );
    forEach( dctAll, [this] (SdObject *obj) -> bool {
      SdGraphTraced *traced = dynamic_cast<SdGraphTraced*>(obj);
      if( traced )
        traced->forsedSubNet( this );
      return true;
      });

    //For each sub net calc and addon minimal distance beatween pairs of sub nets
    for( int i : mSubNet )
      if( i == 0 ) continue;
      else {
        SdDistanceInfo info;
        forEach( dctAll, [this,i,&info] (SdObject *obj) -> bool {
          SdGraphTraced *traced = dynamic_cast<SdGraphTraced*>(obj);
          if( traced )
            traced->getSubNetDistance( i, &info );
          return true;
          });
        if( info.mDestSubNet ) {
          //Link beatween sub nets present. Append it to rat net pair table
          SdRatNetPair pair( info.mSource, info.mDest );
          mRatNet.append( pair );
          //Convert dest sub net to sour
          }

        }
    }
  }




//Get subnet position index (cell number in mSubNet witch contains subNet index)
int SdPItemPlate::getSubNetRef( SdObject *last, const QString netName, SdPoint p, SdStratum s)
  {
  int cell = 0;
  //Scan all component until last and try get subNet
  forEach( dctAll, [ &cell, last, netName, p, s] (SdObject *obj) -> bool {
    if( obj == last ) return false;
    SdGraphTraced *traced = dynamic_cast<SdGraphTraced*>( obj );
    if( traced != nullptr )
      cell = traced->getSubNet( netName, p, s );
    return cell == 0;
    });

  //If subNet found, then return it
  if( cell ) return cell;

  //SubNet not found, create new one
  cell = mSubNet.count();
  mSubNet.append( cell );
  return cell;
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

