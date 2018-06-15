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
  mStratumCount(2),
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








//bool SdPItemPlate::isAvailableSegment(SdPoint start, SdPoint &stop, int stratum, const QString netName, int width)
//  {
//  //Fill rules block
//  SdRuleBlock blk;
//  ruleBlockForNet( stratum, netName, blk );
//  //Change width with given
//  blk.mRules[ruleWireWidth] = width;

//  //Accum barriers
//  QList<QPolygonF> barriers;
//  accumBarriers( barriers, stratum, netName, true, blk );

//  //If there intersection line(start,stop) with any polygon then segment not available
//  //Test if segment intersects with polygon edges
//  QPointF startf(start);
//  QPointF stopf(stop);
//  bool fail = false;
//  for( QPolygonF &poly : barriers ) {
//    if( poly.containsPoint( startf, Qt::OddEvenFill ) ) {
//      //Start point is not available
//      stop = start;
//      return false;
//      }

//    //For each edges test intersection with line
//    for( int i = 0; i < poly.count(); i++ ) {
//      QLineF linef( startf, stopf );
//      if( i == 0 ) {
//        //Final edge
//        if( linef.intersect( QLineF(poly[0],poly[poly.count()-1]), &stopf ) == QLineF::BoundedIntersection )
//          fail = true;
//        }
//      else {
//        //Internal edge
//        if( linef.intersect( QLineF(poly[i-1],poly[i]), &stopf ) == QLineF::BoundedIntersection )
//          fail = true;
//        }
//      }
//    }
//  if( fail )
//    stop = stopf.toPoint();
//  return !fail;
//  }






void SdPItemPlate::accumBarriers(quint64 mask, SdBarrierList &dest, int stratum, SdRuleId toWhich, const SdRuleBlock &rule)
  {
  forEach( mask, [&] (SdObject *obj) -> bool {
    SdGraphTraced *traced = dynamic_cast<SdGraphTraced*>(obj);
    if( traced != nullptr )
      traced->accumBarriers( dest, stratum, toWhich, rule );
    return true;
    } );
  }





//Check rules. fun1 - progress indicator. It calls 10 times per stratum
void SdPItemPlate::checkRules(std::function<bool()> fun1)
  {
  //Remove previous errors
  mRuleErrors.clear();

  //Rules checking in two etaps for each stratum:
  // 1. Check pads with pads
  // 2. Check wires with all
  for( int stratumIndex = 0; stratumIndex < mStratumCount; stratumIndex++ ) {
    //Execute check pads with pads
    //We accum clear pads and pads with own clearance. Then we check all pads with clearance to all clear pads on unit
    // polygon presence. If unit present then append error
    SdBarrierList pads;
    SdBarrierList padsWithClearance;
    //Prepare rule for pads and pads with clearance. We need all rule zero
    SdRuleBlock rule;
    rule.setAllRule( 0 );
    accumBarriers( dctPartImp | dctTraceVia, pads, 1 << stratumIndex, ruleRoadWidth, rule );
    if( fun1() ) return;
    accumBarriers( dctPartImp | dctTraceVia, padsWithClearance, 1 << stratumIndex, rulePadPad, rule );
    if( fun1() ) return;

    //Check all with clearance to all clear pads on unit polygon presence.
    checkIntersection( padsWithClearance, pads, true );
    if( fun1() ) return;

    //Execute check wires with all
    //We accum pads with clearance to wires and wire to pad clearance and wire to wire clearance.
    // Then, we compare wire to pad clearance with pads
    padsWithClearance.clear();
    accumBarriers( dctPartImp | dctTraceVia, padsWithClearance, 1 << stratumIndex, ruleRoadRoad, rule );
    if( fun1() ) return;
    SdBarrierList wires;
    SdBarrierList wiresWithPadClearance;
    SdBarrierList wiresWithWireClearance;
    accumBarriers( dctTraceRoad, wires, 1 << stratumIndex, ruleRoadWidth, rule );
    if( fun1() ) return;
    accumBarriers( dctTraceRoad, wiresWithPadClearance, 1 << stratumIndex, rulePadPad, rule );
    if( fun1() ) return;
    accumBarriers( dctTraceRoad, wiresWithWireClearance, 1 << stratumIndex, ruleRoadRoad, rule );
    if( fun1() ) return;

    //Compare pads with wire clearance to wires
    checkIntersection( padsWithClearance, wires, false );
    if( fun1() ) return;
    //Compare wires with pad clearance to pads
    checkIntersection( wiresWithPadClearance, pads, false );
    if( fun1() ) return;
    //Compare wires to wires with wire clearance
    checkIntersection( wires, wiresWithWireClearance, true );
    if( fun1() ) return;
    }

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
    obj.insert( QStringLiteral("Width"), blk.mRules[ruleRoadWidth] );
    obj.insert( QStringLiteral("PadPad"), blk.mRules[rulePadPad] );
    obj.insert( QStringLiteral("RoadPad"), blk.mRules[ruleRoadPad] );
    obj.insert( QStringLiteral("RoadRoad"), blk.mRules[ruleRoadRoad] );
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
    blk.mRules[ruleRoadWidth] = obj.value( QStringLiteral("Width") ).toInt();
    blk.mRules[rulePadPad] = obj.value( QStringLiteral("PadPad") ).toInt();
    blk.mRules[ruleRoadPad] = obj.value( QStringLiteral("RoadPad") ).toInt();
    blk.mRules[ruleRoadRoad] = obj.value( QStringLiteral("RoadRoad") ).toInt();
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





//Check intersection on barriers and form errorList
void SdPItemPlate::checkIntersection(const SdBarrierList &src, const SdBarrierList &dst, bool excludeSameIndex)
  {
  for( int si = 0; si < src.count(); si++ ) {
    //Compare pad [si] with all pads except same pad
    const SdBarrier &sb = src.at(si);
    bool isConnected = !sb.mNetName.isEmpty();
    for( int di = 0; di < dst.count(); di++ )
      if( !excludeSameIndex || si != di ) {
        const SdBarrier &db = dst.at(di);
        //pads connected to the same net not controlled
        if( !isConnected || sb.mNetName != db.mNetName ) {
          if( sb.mPolygon.intersects( db.mPolygon ) ) {
            //Intersected parts - is error. Intersected polygon over rect is error location
            SdRect overError = sb.mPolygon.intersected(db.mPolygon).boundingRect().toRect();
            //Append error indicator to error list
            mRuleErrors.append( overError );
            }
          }
        }
    }

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




