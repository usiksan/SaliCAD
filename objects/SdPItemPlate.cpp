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
#include "SdPlateNetList.h"

#include <QDebug>
#include <QLineF>
#include <QLine>
#include <algorithm>

SdPItemPlate::SdPItemPlate() :
  SdProjectItem(),
  mStratumCount(2),
  mRatNetDirty(true)
  {
  //Default pcb rules
  mRulesPcb = sdEnvir->mDefaultRules;
  }






SdGraphPartImp *SdPItemPlate::allocPartImp(int *section, SdPItemPart *part, SdPItemComponent *comp, SdPItemSymbol *sym, const SdStringMap &param, SdUndo *undo )
  {
  if( part == nullptr )
    return nullptr;
  //At first, scan all part implements and find match
  SdGraphPartImp *res = nullptr;
  forEach( dctPartImp, [&] (SdObject *obj) -> bool {
    res = dynamic_cast<SdGraphPartImp*>( obj );
    Q_ASSERT( res != nullptr );
    if( res->isSectionFree( section, part, comp, param, sym ) )
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
  res = new SdGraphPartImp( p, &(sdGlobalProp->mPartImpProp), part, comp, param );
  insertChild( res, undo );

  Q_ASSERT( res->isSectionFree( section, part, comp, param, sym ) );
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




//Return over pad circle radius
int SdPItemPlate::getPadOverRadius(const QString pinType) const
  {
  if( mPadAssociation.contains(pinType) )
    return mPadAssociation.pin( pinType ).overCircleRadius();
  return -1;
  }




//Append window for pin pad
void SdPItemPlate::appendPadWindow(SdPolyWindowList &dest, SdPoint p, const QString pinType, int gap, const QTransform &t)
  {
  if( mPadAssociation.contains(pinType) )
    return mPadAssociation.pin( pinType ).appendWindow( dest, p, gap, t );
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
    if( traced && !traced->isSelected() )
      traced->accumNetSegments( netList );
    return true;
    });

  //Build rat net
  netList.buildRatNet( &mRatNet );

  mRatNetDirty = false;
  }





//Draw pcb for trace
void SdPItemPlate::drawTrace(SdContext *ctx, SdStratum curStratum, QString currentNetName)
  {
  //Draw graphics
  forEach( dctPicture, [ctx] (SdObject *obj) -> bool {
    SdGraph *graph = dynamic_cast<SdGraph*>(obj);
    if( graph != nullptr )
      graph->draw(ctx);
    return true;
    } );

  //Draw components without pads
  forEach( dctPartImp, [ctx] (SdObject *obj) -> bool {
    SdGraphPartImp *imp = dynamic_cast<SdGraphPartImp*>(obj);
    if( imp != nullptr )
      imp->drawWithoutPads(ctx);
    return true;
    } );

  //Draw trace elements down stratums
  //Initially, we assume that no current stratum (through stratum)
  SdStratum stratum = stmThrough;
  //If current stratum valid then exclude it from drawing
  if( curStratum.isValid() )
    stratum = stmThrough & (~curStratum.getValue());

  //Draw component pads down stratums
  forEach( dctPartImp, [ctx,stratum] (SdObject *obj) -> bool {
    SdGraphPartImp *imp = dynamic_cast<SdGraphPartImp*>(obj);
    if( imp != nullptr )
      imp->drawPads( ctx, stratum, QString() );
    return true;
    } );

  //Draw roads down stratums
  forEach( dctTraceRoad|dctTraceVia|dctTracePolygon, [ctx,stratum] (SdObject *obj) -> bool {
    SdGraphTraced *trace = dynamic_cast<SdGraphTraced*>(obj);
    if( trace != nullptr )
      trace->drawStratum( ctx, stratum );
    return true;
    } );

  //Draw component pads for current stratum
  if( curStratum.isValid() ) {
    //Net name for highlighting
    stratum = curStratum.getValue();
    forEach( dctPartImp, [ctx,stratum,currentNetName] (SdObject *obj) -> bool {
      SdGraphPartImp *imp = dynamic_cast<SdGraphPartImp*>(obj);
      if( imp != nullptr )
        imp->drawPads( ctx, stratum, currentNetName );
      return true;
      } );

    forEach( dctTraceRoad|dctTraceVia|dctTracePolygon, [ctx,stratum] (SdObject *obj) -> bool {
      SdGraphTraced *trace = dynamic_cast<SdGraphTraced*>(obj);
      if( trace != nullptr )
        trace->drawStratum( ctx, stratum );
      return true;
      } );
    }
  }






//Set new rules
void SdPItemPlate::ruleSet(const SdRuleBlock &pcb, const SdRuleBlockMap &map, SdUndo *undo )
  {
  //TODO D057 Append undo rules
  mRulesPcb = pcb;
  mRulesMap = map;
  }





int SdPItemPlate::ruleForNet(const QString netName, SdRuleId ruleId ) const
  {
  return mRulesMap.value( netName, mRulesPcb ).rule( ruleId, mRulesPcb );
  }




//Build rules block for given net
void SdPItemPlate::ruleBlockForNet( const QString netName, SdRuleBlock &blockDest)
  {
  mRulesMap.value( netName, mRulesPcb ).getRuleBlock( blockDest, mRulesPcb );
  }








//Scan all trace objects and build barriers list
// mask - scaning objects mask
// dest - barrier list to which will accum new barriers
// stratum - stratum list on which will be barriers
// toWhich - destignation object type to which barriers will be builded
// rule    - source object rules
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
    obj.insert( iter.key(), iter.value().write() );
  return obj;
  }









void SdPItemPlate::readRuleMap(const QJsonObject obj)
  {
  mRulesMap.clear();
  for( auto iter = obj.constBegin(); iter != obj.constEnd(); iter++ ) {
    SdRuleBlock block;
    block.read( iter.value().toObject() );
    mRulesMap.insert( iter.key(), block );
    }
  }








typedef QList<SdGraphPartImp*> SdPartImpList;
//Renumeration implements
void SdPItemPlate::renumeration()
  {
  //Renumeration perform on comp ident prefix base.
  //For all component with same prefix base components renumerated
  // from top to bottom and from left to right

  //In this map will contains all implements by ident prefix
  QMap<QString, SdPartImpList* > map;

  //Accum all implements
  forEach( dctPartImp, [&map] (SdObject *obj) -> bool {
    SdGraphPartImp *imp = dynamic_cast<SdGraphPartImp*>(obj);
    if( imp ) {
      //Retrive implement prefix
      QString prefix = imp->getIdentPrefix();
      //Test if prefix already contained in map
      if( map.contains(prefix) )
        //Prefix already contained in map. Append to list
        map.value(prefix)->append( imp );
      else {
        //No prefix in map yet
        SdPartImpList *list = new SdPartImpList();
        list->append( imp );
        map.insert( prefix, list );
        }
      }
    return true;
    });

  //For each prefix sort components
  for( auto i = map.begin(); i != map.end(); i++ ) {
    SdPartImpList *list = i.value();
    if( list->count() > 1 ) {
      //Perform sorting from top to bottom and from left to right
      std::stable_sort( list->begin(), list->end(), [] ( SdGraphPartImp *imp1, SdGraphPartImp *imp2 ) -> bool {
        if( imp1 == nullptr || imp2 == nullptr )
          return false;
        return imp1->compareRenumeration( imp2 );
        });
      }

    //Sorted. Assign new indexes
    int logNum = 1;
    for( SdGraphPartImp *imp : (*list) )
      imp->setIdentIndex( logNum++ );
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

  //Write pcb rules
  obj.insert( QStringLiteral("PcbRules"), mRulesPcb.write() );
  }





void SdPItemPlate::readObject(SdObjectMap *map, const QJsonObject obj)
  {
  SdProjectItem::readObject( map, obj );
  mPartRow.read( QStringLiteral("PartRow"), obj );

  //Read pad assotiation
  mPadAssociation.readObject( map, obj.value( QStringLiteral("PadAssociation") ).toObject() );

  //Read rules map
  readRuleMap( obj.value(QStringLiteral("RulesMap")).toObject() );

  //Read pcb rules
  mRulesPcb.read( obj.value(QStringLiteral("PcbRules")).toObject() );
  }




