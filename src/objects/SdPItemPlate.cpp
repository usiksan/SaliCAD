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
#include "SdStratum.h"
#include "SdPulsar.h"
#include "SdGraphLinearRect.h"
#include "SdGraphLinearRegion.h"
#include "SdGraphLinearCircle.h"
#include "Sd3dDraw.h"
#include "SdJsonIO.h"

#include <QDebug>
#include <QLineF>
#include <QLine>
#include <algorithm>

SdPItemPlate::SdPItemPlate() :
  SdProjectItem(),
  mStratumCount(2),
  mRatNetDirty(true),
  mTraceGrid(-1,-1),         //Current trace grid
  mTraceCursorGrid(false),   //Enable cursor grid align when trace
  mPlaceGrid(-1,-1),         //Current place grid
  mPlaceCursorGrid(true)     //Enable cursor grid align when place
  {
  //Default pcb rules
  mRulesPcb = sdEnvir->mDefaultRules;
  }




void SdPItemPlate::setStratumCount(int sc)
  {
  //Count of stratum must be 1, 2, 4, 6 and etc
  mStratumCount = (sc >> 1) << 1;
  if( mStratumCount == 0 )
    mStratumCount = 1;
  if( mStratumCount > 30 )
    mStratumCount = 30;
  mRatNetDirty = true;
  }





int SdPItemPlate::stratumMask() const
  {
  return SdStratum::stratumStack( mStratumCount );
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

  bool isFree = res->isSectionFree( section, part, comp, param, sym );
  Q_ASSERT( isFree );
  return res;
  }




//Return pad
SdPad SdPItemPlate::getPad(const QString pinType) const
  {
  //If pad present in pad association then take it from association
  if( mPadAssociation.contains(pinType) )
    return mPadAssociation.pin( pinType );
  //else create pad from description
  return sdEnvir->getPad( pinType );
  }





void SdPItemPlate::drawPad(SdContext *dc, SdPoint p, const QString pinType, int stratum) const
  {
  getPad( pinType ).draw( dc, p, stratum );
  }





//Setup new map and name
void SdPItemPlate::setPadAssociation(const QString nm, const SdPadMap &map, SdUndo *undo)
  {
  mPadAssociation.setMap( nm, map, undo );
  }




QPolygonF SdPItemPlate::getPadPolygon(SdPoint p, const QString pinType, int addon) const
  {
  //create polygon from pad
  return getPad( pinType ).polygon(p, addon);
  }




//Return over pad circle radius
int SdPItemPlate::getPadOverRadius(const QString pinType) const
  {
  return getPad( pinType ).overCircleRadius();
  }




//Append window for pin pad
void SdPItemPlate::appendPadWindow(SdPolyWindowList &dest, SdPoint p, const QString pinType, int gap, const QTransform &t) const
  {
  getPad( pinType ).appendWindow( dest, p, gap, t );
  }




//!
//! \brief appendPadHoles Accum holes description into faceList
//! \param p              Position of pad
//! \param pinType        Pad description
//! \param model          Model which accumulate coord vertexes
//! \param faceList       Face list for holding holes (single face for single hole)
//! \param stratum        Stratum for layers
//! \param map            Map for holes conversion
//!
void SdPItemPlate::appendPadHoles(SdPoint p, const QString pinType, Sd3drModel &model, Sd3drFaceList &faceList, SdStratum stratum, const QMatrix4x4 &map) const
  {
  getPad( pinType ).appendPadHoles( p, model, faceList, stratum, map );
  }







//Accumulate net segments to given net container
void SdPItemPlate::accumNetSegments(SdPlateNetContainer *container)
  {
  forEach( dctAll, [container] (SdObject *obj) -> bool {
    SdGraphTraced *traced = dynamic_cast<SdGraphTraced*>(obj);
    if( traced && !traced->isSelected() )
      traced->accumNetSegments( container );
    return true;
    });
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
  accumNetSegments( &netList );

  //Build rat net
  netList.buildRatNet( &mRatNet );

  mRatNetDirty = false;

  SdPulsar::sdPulsar->emitSetTracingStatus( unconnectedNetCount(), ruleErrorsCount() );
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
      if( trace != nullptr && trace->getSelector() == nullptr )
        trace->drawStratum( ctx, stratum );
      return true;
      } );
    }
  }






//Set new rules
void SdPItemPlate::ruleSet(const SdRuleBlock &pcb, const SdRuleBlockMap &map, SdUndo *undo )
  {
  if( undo )
    undo->rule( &mRulesPcb, &mRulesMap );
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




//!
//! \brief ruleForNetSet Set new rule for single net
//! \param netName       Net name for which rule will be set
//! \param ruleId        Rule id
//! \param val           Value for rule
//! \param undo          Undo
//!
void SdPItemPlate::ruleForNetSet(const QString netName, SdRuleId ruleId, int val, SdUndo *undo)
  {
  if( undo )
    undo->rule( &mRulesPcb, &mRulesMap );
  //Rule for net
  SdRuleBlock rules;
  //If rule for net already exist in map we extract it for change
  if( mRulesMap.contains( netName ) )
    rules = mRulesMap.value( netName );
  //Update rule value
  rules.mRules[ruleId] = val;
  //Write rules to map
  mRulesMap.insert( netName, rules );
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
    accumBarriers( dctPartImp | dctTraceVia, pads, 1 << stratumIndex, ruleFree, rule );
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
    accumBarriers( dctTraceRoad, wires, 1 << stratumIndex, ruleFree, rule );
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

  qDeleteAll( map );
  }





void SdPItemPlate::rebuild3dModel()
  {
  //Build pcb
  m3dModel.clear();
  SdPointList pointList;
  //At first, find pcb contour
  SdLayer *pcbLayer = sdEnvir->getLayer( LID0_PCB );
  forEach( dctLines, [&pointList, pcbLayer] (SdObject *obj) -> bool {
    SdPtr<SdGraphLinearRect> rect(obj);
    if( rect.isValid() && rect->isMatchLayer(pcbLayer) ) {
      //There we found pcb rectangle form
      //Draw it and cancel iteration
      pointList = rect->getPointList();
      return false;
      }
    SdPtr<SdGraphLinearRegion> region(obj);
    if( region.isValid() && region->isMatchLayer(pcbLayer) ) {
      //There we found pcb region form
      //Draw it and cancel iteration
      pointList = region->getPointList();
      return false;
      }
    return true;
    } );

  //If no pcb contour then we do nothing
  if( pointList.count() > 2 ) {
    Sd3drFace pcbTop;
    for( auto p : qAsConst(pointList) ) {
      QVector3D v( p );
      v /= 1000.0;
      v.setZ( pcb3dZLevel );
      pcbTop.append( m3dModel.vertexAppend(v) );
      }

    //At second we scan all throught holes
    SdLayer *holeLayer = sdEnvir->getLayer( LID0_HOLE );
    Sd3drFaceList holes;
    QMatrix4x4 map;
    map.translate( 0, 0, pcb3dZLevel );
    forEach( dctLines|dctPartImp, [&holes,holeLayer,this,map] ( SdObject *obj ) -> bool {
      SdPtr<SdGraphLinear> linear(obj);
      if( linear.isValid() && linear->isMatchLayer(holeLayer) )
        //Append figure to hole list (lines not generate holes)
        linear->accumHoles( m3dModel, holes, stmThrough, map );
      else {
        SdPtr<SdGraphPartImp> partImp(obj);
        if( partImp.isValid() )
          //Convert circle to point region
          partImp->accumHoles( m3dModel, holes, stmThrough, map );
        }
      return true;
      });


    Sd3drBody pcbBody;
    QMatrix4x4 shift;
    shift.translate( 0, 0, -pcb3dThickness );
    Sd3drFace pcbBot = m3dModel.faceDuplicate( pcbTop, shift );
    pcbBody.faceAppend( m3dModel.faceListWall( pcbTop, pcbBot, true ) );
    pcbBody.colorSet( sdEnvir->getSysColor( sc3dPcb ) );

    Sd3drFaceList pcbTopList = m3dModel.faceListHolesXY( pcbTop, holes );
    Sd3drFaceList pcbBotList = m3dModel.faceListDuplicate( pcbTopList, shift );
    pcbBody.faceAppend( pcbTopList );
    pcbBody.faceAppend( pcbBotList );
    //pcbBody.faceAppend( holes );

    Sd3drInstance inst;
    inst.add( pcbBody );
    inst.addCopy( QMatrix4x4() );

    m3dModel.instanceAppend( inst );
    }

  }







QString SdPItemPlate::getType() const
  {
  return QStringLiteral( SD_TYPE_PLATE );
  }




SdClass SdPItemPlate::getClass() const
  {
  return dctPlate;
  }




void SdPItemPlate::detach(SdUndo *undo)
  {
  //On detach we delete all contents
  deleteAll( undo );
  }



//!
//! \brief json Overloaded function to write object content into json writer
//!             Overrided function
//! \param js   Json writer
//!
void SdPItemPlate::json(SdJsonWriter &js) const
  {
  js.jsonRect( QStringLiteral("PartRow"), mPartRow );

  //Write pad assotiation
  js.jsonObject( js, QStringLiteral("PadAssociation"), mPadAssociation );

  //Write rules map
  js.jsonMap( js, QStringLiteral("RulesMap"), mRulesMap );

  //Write pcb rules
  js.jsonObject( js, QStringLiteral("PcbRules"), mRulesPcb );

  js.jsonPoint( QStringLiteral("TraceGrid"), mTraceGrid );         //Current trace grid
  js.jsonBool( QStringLiteral("TraceCursorGrid"), mTraceCursorGrid );  //Enable cursor grid align when trace
  js.jsonPoint( QStringLiteral("PlaceGrid"), mPlaceGrid );         //Current place grid
  js.jsonBool( QStringLiteral("PlaceCursorGrid"), mPlaceCursorGrid);   //Enable cursor grid align when place

  SdProjectItem::json( js );
  }




//!
//! \brief json Overloaded function to read object content from json reader
//!             Overrided function
//! \param js   Json reader
//!
void SdPItemPlate::json(const SdJsonReader &js)
  {
  js.jsonRect( QStringLiteral("PartRow"), mPartRow );

  //Write pad assotiation
  js.jsonObject( js, QStringLiteral("PadAssociation"), mPadAssociation );

  //Write rules map
  js.jsonMap( js, QStringLiteral("RulesMap"), mRulesMap );

  //Write pcb rules
  js.jsonObject( js, QStringLiteral("PcbRules"), mRulesPcb );

  js.jsonPoint( QStringLiteral("TraceGrid"), mTraceGrid );         //Current trace grid
  js.jsonBool( QStringLiteral("TraceCursorGrid"), mTraceCursorGrid );  //Enable cursor grid align when trace
  js.jsonPoint( QStringLiteral("PlaceGrid"), mPlaceGrid );         //Current place grid
  js.jsonBool( QStringLiteral("PlaceCursorGrid"), mPlaceCursorGrid);   //Enable cursor grid align when place

  SdProjectItem::json( js );
  }




QString SdPItemPlate::getIconName() const
  {
  return QStringLiteral(":/pic/iconPlate.png");
  }





void SdPItemPlate::draw3d(QOpenGLFunctions_2_0 *f)
  {
  //Draw pcb model
  m3dModel.draw3d( f );

  //Draw all components
  forEach( dctPartImp, [f] (SdObject *obj) -> bool {
    SdPtr<SdGraphPartImp> part(obj);
    if( part.isValid() )
      part->draw3d( f );
    return true;
    });

  //SdProjectItem::draw3d( f );
  }




quint64 SdPItemPlate::getAcceptedObjectsMask() const
  {
  return dctPlateObjects;
  }





//Set layers usage
void SdPItemPlate::setLayerUsage()
  {
  //Default layers usage
  SdProjectItem::setLayerUsage();
  //Setup usage for cached layers
  sdEnvir->setLayerUsage( mStratumCount );
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



