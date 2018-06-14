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
#include "SdPItemComponent.h"
#include "SdProject.h"
#include "SdContext.h"
#include "SdEnvir.h"
#include <QDebug>
#include <QLineF>
#include <QLine>

SdPItemPlate::SdPItemPlate() :
  SdProjectItem(),
  mRatNetDirty(true)
  {
  SdRuleBlock rules;
  //Default pcb rules
  mRules.append( sdEnvir->mDefaultRules );
  mRulesMap.insert( RULE_BLOCK_PCB, RULE_BLOCK_PCB_ID );
  //Default layers rules
  for( int i = 0; i < 30; i++ ) {
    mRules.append( rules );
    mRulesMap.insert( RULE_BLOCK_LAYER(i), RULE_BLOCK_LAYER_ID + i );
    }
  //Default net rules
  mRules.append( rules );
  mRulesMap.insert( RULE_BLOCK_DEF, RULE_BLOCK_DEF_ID );
  }






SdGraphPartImp *SdPItemPlate::allocPartImp(int *section, SdPItemPart *part, SdPItemComponent *comp, SdPItemSymbol *sym, SdUndo *undo )
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





void SdPItemPlate::drawPad(SdContext *dc, SdPoint p, const QString pinType, int stratum) const
  {
  if( mPadAssociation.contains(pinType) )
    mPadAssociation.pin( pinType ).draw( dc, p, stratum );
  }





//Setup new map and name
void SdPItemPlate::setPadAssociation(const QString nm, const SdPadMap &map, SdUndo *undo)
  {
  mPadAssociation.setMap( nm, map, undo );
  }




QPolygonF SdPItemPlate::getPadPolygon(SdPoint p, const QString pinType, int addon) const
  {
  if( mPadAssociation.contains(pinType) )
    return mPadAssociation.pin( pinType ).polygon(p, addon);
  return SdPad().polygon(p, addon);
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





int SdPItemPlate::ruleForNet(int stratum, const QString netName, SdRuleId ruleId )
  {
  int stratumIndex = SdStratum::stratumIndex(stratum);
  int id;
  if( netName.isEmpty() )
    id = RULE_BLOCK_LAYER_ID + stratumIndex;
  else {
    //Test for net on stratum
    QString netNameWithStratum = RULE_BLOCK_NET( stratumIndex, netName );
    id = mRulesMap.value( netNameWithStratum, -1 );
    if( id < 0 ) {
      //Test for net only
      id = mRulesMap.value( netName, -1 );
      //Failed key in rules map
      if( id < 0 )
        //Append default net id
        mRulesMap.insert( netName, id = RULE_BLOCK_DEF_ID );
      //Append stratum variant
      mRulesMap.insert( netNameWithStratum, id );
      }
    }
  return ruleFromId( stratumIndex, id, ruleId );
  }




//Build rules block for given net
void SdPItemPlate::ruleBlockForNet(int stratum, const QString netName, SdRuleBlock &blockDest)
  {
  int stratumIndex = SdStratum::stratumIndex(stratum);
  //Test for net on stratum
  QString netNameWithStratum = RULE_BLOCK_NET( stratumIndex, netName );
  int id = mRulesMap.value( netNameWithStratum, -1 );
  if( id < 0 ) {
    //Test for net only
    id = mRulesMap.value( netName, -1 );
    //Failed key in rules map
    if( id < 0 )
      //Append default net id
      mRulesMap.insert( netName, id = RULE_BLOCK_DEF_ID );
    //Append stratum variant
    mRulesMap.insert( netNameWithStratum, id );
    }
  for( int i = 0; i < ruleLast; i++ )
    blockDest.mRules[i] = ruleFromId( stratumIndex, id, i );
  }








bool SdPItemPlate::isAvailableSegment(SdPoint start, SdPoint &stop, int stratum, const QString netName, int width)
  {
  //Fill rules block
  SdRuleBlock blk;
  ruleBlockForNet( stratum, netName, blk );
  //Change width with given
  blk.mRules[ruleWireWidth] = width;

  //Accum barriers
  QList<QPolygonF> barriers;
  accumBarriers( barriers, stratum, netName, true, blk );

  //If there intersection line(start,stop) with any polygon then segment not available
  //Test if segment intersects with polygon edges
  QPointF startf(start);
  QPointF stopf(stop);
  bool fail = false;
  for( QPolygonF &poly : barriers ) {
    if( poly.containsPoint( startf, Qt::OddEvenFill ) ) {
      //Start point is not available
      stop = start;
      return false;
      }

    //For each edges test intersection with line
    for( int i = 0; i < poly.count(); i++ ) {
      QLineF linef( startf, stopf );
      if( i == 0 ) {
        //Final edge
        if( linef.intersect( QLineF(poly[0],poly[poly.count()-1]), &stopf ) == QLineF::BoundedIntersection )
          fail = true;
        }
      else {
        //Internal edge
        if( linef.intersect( QLineF(poly[i-1],poly[i]), &stopf ) == QLineF::BoundedIntersection )
          fail = true;
        }
      }
    }
  if( fail )
    stop = stopf.toPoint();
  return !fail;
  }






void SdPItemPlate::accumBarriers(QList<QPolygonF> &dest, int stratum, const QString netName, bool toWire, const SdRuleBlock &rule)
  {
  forEach( dctTraced, [&] (SdObject *obj) -> bool {
    SdGraphTraced *traced = dynamic_cast<SdGraphTraced*>(obj);
    if( traced != nullptr )
      traced->accumBarriers( dest, stratum, netName, toWire, rule );
    return true;
    } );
  }





//Check rules
void SdPItemPlate::checkRules()
  {
  //Remove previous errors
  mRuleErrors.clear();

  }





//Draw rule error indicators
void SdPItemPlate::drawRuleErrors(SdContext *dc) {
  dc->setPen( 0, sdEnvir->getSysColor(scRuleErrors), dltSolid );
  for( const SdRect &r : mRuleErrors )
    dc->rect( r );
  }





QJsonObject SdPItemPlate::writeRuleMap() const
  {
  QJsonObject obj;
  for( auto iter = mRulesMap.cbegin(); iter != mRulesMap.cend(); iter++ )
    obj.insert( iter.key(), iter.value() );
  return obj;
  }




QJsonArray SdPItemPlate::writeRuleTable() const
  {
  QJsonArray ar;
  for( const SdRuleBlock &blk : mRules ) {
    QJsonObject obj;
    obj.insert( QStringLiteral("Width"), blk.mRules[ruleWireWidth] );
    obj.insert( QStringLiteral("PadPad"), blk.mRules[rulePadPad] );
    obj.insert( QStringLiteral("WirePad"), blk.mRules[ruleWirePad] );
    obj.insert( QStringLiteral("WireWire"), blk.mRules[ruleWireWire] );
    obj.insert( QStringLiteral("Top"), blk.mTopBlock );
    ar.append( obj );
    }
  return ar;
  }





void SdPItemPlate::readRuleMap(const QJsonObject obj)
  {
  mRulesMap.clear();
  for( auto iter = obj.constBegin(); iter != obj.constEnd(); iter++ )
    mRulesMap.insert( iter.key(), iter.value().toInt() );
  }




void SdPItemPlate::readRuleTable(const QJsonArray ar)
  {
  mRules.clear();
  SdRuleBlock blk;
  for( auto iter = ar.constBegin(); iter != ar.constEnd(); iter++ ) {
    QJsonObject obj = iter->toObject();
    blk.mRules[ruleWireWidth] = obj.value( QStringLiteral("Width") ).toInt();
    blk.mRules[rulePadPad] = obj.value( QStringLiteral("PadPad") ).toInt();
    blk.mRules[ruleWirePad] = obj.value( QStringLiteral("WirePad") ).toInt();
    blk.mRules[ruleWireWire] = obj.value( QStringLiteral("WireWire") ).toInt();
    blk.mTopBlock = obj.value( QStringLiteral("Top") ).toInt();
    mRules.append( blk );
    }
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





//Retrive rule starting with blockId
int SdPItemPlate::ruleFromId(int stratumIndex, int blockId, int ruleId )
  {
  //Retrive rule with starting block
  int val = mRules.at(blockId).mRules[ruleId];
  //Test, if rule default and blockId not top, then
  // switch to top block and repeate retrive rule
  while( val < 0 && blockId != 0 ) {
    //Get top block id
    int top = mRules.at(blockId).mTopBlock;
    //For layer block use stratumIndex to get blockId, for other block - use block
    if( top == RULE_BLOCK_LAYER_ID ) blockId = RULE_BLOCK_LAYER_ID + stratumIndex;
    else blockId = top;
    //Retrive rule from new block
    val = mRules.at(blockId).mRules[ruleId];
    }
  return val;
  }






void SdPItemPlate::writeObject(QJsonObject &obj) const
  {
  SdProjectItem::writeObject( obj );
  mPartRow.write( QStringLiteral("PartRow"), obj );

  //Write pad assotiation
  obj.insert( QStringLiteral("PadAssociation"), mPadAssociation.write() );

  //Write rules map
  obj.insert( QStringLiteral("RulesMap"), writeRuleMap() );

  //Write rules array
  obj.insert( QStringLiteral("Rules"), writeRuleTable() );
  }





void SdPItemPlate::readObject(SdObjectMap *map, const QJsonObject obj)
  {
  SdProjectItem::readObject( map, obj );
  mPartRow.read( QStringLiteral("PartRow"), obj );

  //Read pad assotiation
  mPadAssociation.readObject( map, obj.value( QStringLiteral("PadAssociation") ).toObject() );

  //Read rules map
  readRuleMap( obj.value(QStringLiteral("RulesMap")).toObject() );

  //Read rules table
  readRuleTable( obj.value(QStringLiteral("Rules")).toArray() );
  }




