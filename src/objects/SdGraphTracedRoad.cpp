/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Tracing road segment presented by end points, net name, stratum and road width
*/
#include "SdGraphTracedRoad.h"
#include "SdEnvir.h"
#include "SdSegment.h"
#include "SdContext.h"
#include "SdSelector.h"
#include "SdPlateNetList.h"
#include "SdPItemPlate.h"
#include "SdUndo.h"
#include <QTransform>

SdGraphTracedRoad::SdGraphTracedRoad()
  {

  }

SdGraphTracedRoad::SdGraphTracedRoad(const SdPropRoad &prp, SdPoint a, SdPoint b) :
  SdGraphTraced(),
  mProp(prp),
  mSegment(a,b),
  mFly(flyP1P2)
  {

  }




void SdGraphTracedRoad::setSegment(SdPoint a, SdPoint b, SdUndo *undo)
  {
  if( undo )
    undo->road( &(mProp.mWidth), mSegment.ptrP1(), mSegment.ptrP2() );
  mSegment.set( a, b );
  }




//Split road on two roads with p as division point
void SdGraphTracedRoad::splitRoad(SdPoint p, SdUndo *undo)
  {
  SdGraphTracedRoad *road = new SdGraphTracedRoad( mProp, p, mSegment.getP2() );
  setSegment( mSegment.getP1(), p, undo );
  getPlate()->insertChild( road, undo );
  }






//Union consequent segments
void SdGraphTracedRoad::utilize(SdUndo *undo)
  {
  //Remove zero segment
  if( mSegment.getP1() == mSegment.getP2() )
    deleteObject( undo );
  else {
    saveState( undo );
    utilizeAtEnd( mSegment.getP1(), undo );
    utilizeAtEnd( mSegment.getP2(), undo );
    }
  }





QString SdGraphTracedRoad::getType() const
  {
  return QStringLiteral(SD_TYPE_GRAPH_TRACE_ROAD);
  }




quint64 SdGraphTracedRoad::getClass() const
  {
  return dctTraceRoad;
  }




void SdGraphTracedRoad::cloneFrom(const SdObject *src)
  {
  //Clone top
  SdGraphTraced::cloneFrom(src);
  //Clone current
  const SdGraphTracedRoad *road = dynamic_cast<const SdGraphTracedRoad*>(src);
  if( road != nullptr ) {
    mSegment = road->mSegment;
    mProp    = road->mProp;
    }
  }



void SdGraphTracedRoad::writeObject(QJsonObject &obj) const
  {
  //Write top
  SdGraphTraced::writeObject( obj );
  //Members of this class
  mSegment.writeSegment( obj );
  mProp.write( obj );
  }



void SdGraphTracedRoad::readObject(SdObjectMap *map, const QJsonObject obj)
  {
  //Read top
  SdGraphTraced::readObject( map, obj );
  //Members of this class
  mSegment.readSegment( obj );
  mProp.read( obj );
  }




void SdGraphTracedRoad::saveState(SdUndo *undo)
  {
  undo->road( &(mProp.mWidth), mSegment.ptrP1(), mSegment.ptrP2() );
  }




void SdGraphTracedRoad::moveComplete(SdPoint grid, SdUndo *undo)
  {
  Q_UNUSED(grid)
  if( mFly == flyP1_45 || mFly == flyP2_45 ) {
    //Divide segment on two segments
    SdPoint m = mSegment.vertex45();
    if( m != mSegment.getP2() ) {
      SdGraphTracedRoad *road = new SdGraphTracedRoad( mProp, mSegment.getP1(), m );
      getPlate()->insertChild( road, undo );
      mSegment.setP1( m );
      mFly = flyP1P2;
      road->selectByPoint( m, getSelector() );
      }
    }
  if( mSegment.isZero() )
    //This road segment is zero lenght. Delete self
    getPlate()->deleteChild( this, undo );
  }




void SdGraphTracedRoad::move(SdPoint offset)
  {
  if( mFly == flyP1P2 ) mSegment.move( offset );
  else if( mFly == flyP1 || mFly == flyP1_45 ) mSegment.moveP1( offset );
  else if( mFly == flyP2 || mFly == flyP2_45 ) mSegment.moveP2( offset );
  }





//Properties service [Изменение свойствами]
//Set properties of this object from prop
void SdGraphTracedRoad::setProp(SdPropSelected &prop)
  {
  //From global we can set only width
  mProp.mWidth = prop.mRoadProp.mWidth;
  }




//Get (append) properties from this object to prop
void SdGraphTracedRoad::getProp(SdPropSelected &prop)
  {
  prop.mRoadProp.append( mProp );
  prop.mFilledPropMask |= spsRoadProp;
  }




void SdGraphTracedRoad::selectByPoint(const SdPoint p, SdSelector *selector)
  {
  SdLayer *layer = getLayer();
  if( layer != nullptr && layer->isEdited() ) {
    if( !getSelector() ) {
      //Not selected yet
      if( mSegment.isPointOn( p ) ) {
        //Test fixing variant
        if( mSegment.getP1() == p ) {
          if( mSegment.orientation() == SdOrientation::sorAny ) mFly = flyP1;
          else mFly = flyP1_45;
          }
        else if( mSegment.getP2() == p ) {
          if( mSegment.orientation() == SdOrientation::sorAny ) mFly = flyP2;
          else mFly = flyP2_45;
          }
        else mFly = flyP1P2;
        selector->insert( this );
        }
      }
    else {
      //Already selected
      if( mFly != flyP1P2 && mSegment.isPointOn( p ) ) {
        //If one point if fixed, then check new fix condition
        //Если одна точка фиксирована, проверяем новое условие фиксации
        if( mSegment.getP1() == p && (mFly == flyP2 || mFly == flyP2_45) ) mFly = flyP1P2;
        else if( mSegment.getP2() == p && (mFly == flyP1 || mFly == flyP1_45) ) mFly = flyP1P2;
        }
      }
    }
  }





void SdGraphTracedRoad::selectByRect(const SdRect &r, SdSelector *selector)
  {
  SdLayer *layer = getLayer();
  if( layer != nullptr && layer->isEdited() ) {
    if( !getSelector() ) {
      //Not selected yet
      if( r.isAccross( mSegment ) ) {
        if( r.isPointInside(mSegment.getP1()) && !r.isPointInside(mSegment.getP2()) ) {
          if( mSegment.orientation() == SdOrientation::sorAny ) mFly = flyP1;
          else mFly = flyP1_45;
          }
        else if( r.isPointInside(mSegment.getP2()) && !r.isPointInside(mSegment.getP1()) ) {
          if( mSegment.orientation() == SdOrientation::sorAny ) mFly = flyP2;
          else mFly = flyP2_45;
          }
        else mFly = flyP1P2;
        selector->insert( this );
        }
      }
    else {
      //Already selected
      if( r.isAccross( mSegment ) ) {
        //If inside selecting rect then check if was only one end selected then select both
        if( r.isPointInside(mSegment.getP1()) && (mFly == flyP2 || mFly == flyP2_45) ) mFly = flyP1P2;
        else if( r.isPointInside(mSegment.getP2()) && (mFly == flyP1 || mFly == flyP1_45) ) mFly = flyP1P2;
        }
      }
    }
  }




void SdGraphTracedRoad::select(SdSelector *selector)
  {
  if( selector != nullptr )
    selector->insert( this );
  }






void SdGraphTracedRoad::prepareMove(SdUndo *undo)
  {
  Q_UNUSED(undo)
  Q_ASSERT( getSelector() != nullptr );
  bool tryP1 = mFly == flyP1P2 || mFly == flyP1 || mFly == flyP1_45;
  bool tryP2 = mFly == flyP1P2 || mFly == flyP2 || mFly == flyP2_45;
  //for flying point accum roads connected to end point
  getPlate()->forEach( dctTraceRoad | dctTraceVia, [this,tryP1,tryP2] (SdObject *obj) -> bool {
    SdGraphTraced *trace = dynamic_cast<SdGraphTracedRoad*>(obj);
    if( trace != nullptr && trace->isMatchNetAndStratum( mProp.mNetName.str(), mProp.mStratum ) && trace != this ) {
      if( tryP1 )
        trace->selectByPoint( mSegment.getP1(), getSelector() );
      if( tryP2 )
        trace->selectByPoint( mSegment.getP2(), getSelector() );
      }
    return true;
    });
  }






void SdGraphTracedRoad::setLayerUsage()
  {
  //Layer of road
  SdLayer *layer = getLayer();
  if( layer != nullptr )
    layer->setUsage();
  }




bool SdGraphTracedRoad::isVisible()
  {
  //Layer of road
  SdLayer *layer = getLayer();
  return layer != nullptr && layer->isVisible();
  }



SdRect SdGraphTracedRoad::getOverRect() const
  {
  return mSegment.getOverRect();
  }



int SdGraphTracedRoad::behindCursor(SdPoint p)
  {
  //Определить состояние объекта под курсором
  if( isVisible() && mSegment.isPointOn( p ) )
    return getSelector() ? SEL_ELEM : UNSEL_ELEM;
  return 0;
  }




bool SdGraphTracedRoad::getInfo(SdPoint p, QString &info, bool extInfo)
  {
  Q_UNUSED(extInfo)
  if( behindCursor( p ) ) {
    info = QObject::tr("Net: ") + mProp.mNetName.str();
    return true;
    }
  return false;
  }





//Find snap point on object
void SdGraphTracedRoad::snapPoint(SdSnapInfo *snap)
  {
  if( snap->mStratum.match( mProp.mStratum ) ) {
    if( snap->match(snapNearestNet) ) {
      snap->test( this, mSegment.getP1(), snapNearestNet );
      snap->test( this, mSegment.getP2(), snapNearestNet );
      }
    if( snap->match(snapNearestNetNet) && snap->mNetName == mProp.mNetName.str() ) {
      snap->test( this, mSegment.getP1(), snapNearestNetNet );
      snap->test( this, mSegment.getP2(), snapNearestNetNet );
      }
    if( snap->match(snapMidPoint) ) {
      snap->test( this, mSegment.middle(), snapMidPoint );
      }
    if( snap->match(snapEndPoint) ) {
      double distance;
      if( snap->isCandidate(mSegment.getP1(), distance) ) {
        //Good candidate, check end point connections
        SdSelector sel;
        accumLinkedTrace( this, mSegment.getP1(), mProp.mNetName.str(), &sel );
        if( sel.count() == 0 || (sel.count() == 1 && sel.first()->getClass() == dctTraceRoad) )
          snap->test( this, mSegment.getP1(), snapEndPoint );
        }
      if( snap->isCandidate( mSegment.getP2(), distance ) ) {
        //Good candidate, check end point connections
        SdSelector sel;
        accumLinkedTrace( this, mSegment.getP2(), mProp.mNetName.str(), &sel );
        if( sel.count() == 0 || (sel.count() == 1 && sel.first()->getClass() == dctTraceRoad) )
          snap->test( this, mSegment.getP2(), snapEndPoint );
        }
      }
    }
  }





bool SdGraphTracedRoad::isPointOnNet(SdPoint p, SdStratum stratum, QString *netName, int *destStratum)
  {
  if( mProp.mStratum.match( stratum ) && mSegment.isPointOn( p ) ) {
    if( *netName == mProp.mNetName.str() )
      *destStratum |= mProp.mStratum.getValue();
    else {
      *destStratum = mProp.mStratum.getValue();
      *netName = mProp.mNetName.str();
      }
    return true;
    }
  return false;
  }







void SdGraphTracedRoad::accumNetSegments(SdPlateNetList &netList ) const
  {
  netList.addNetSegment( mProp.mNetName.str(), mProp.mStratum, mSegment.getP1(), mSegment.getP2() );
  }





void SdGraphTracedRoad::drawStratum(SdContext *dcx, int stratum)
  {
  if( mProp.mStratum & stratum ) {
    //Layer of road
    SdLayer *layer = getLayer();
    if( layer != nullptr && layer->isVisible() ) {
      dcx->setPen( mProp.mWidth.getValue(), layer, dltSolid );
      if( mFly == flyP1_45 || mFly == flyP2_45 ) {
        //Draw segment with vertex
        SdPoint m = mSegment.vertex45();
        dcx->line( mSegment.getP1(), m );
        if( m != mSegment.getP2() )
          dcx->line( m, mSegment.getP2() );
        }
      else
        //Draw simple line
        dcx->line( mSegment.getP1(), mSegment.getP2() );
      }
    }
  }




void SdGraphTracedRoad::accumBarriers(SdBarrierList &dest, int stratum, SdRuleId toWhich, const SdRuleBlock &blk) const
  {
  //Make around line actagon
  //
  //   1  --------------  2
  // 0  /                \ 3
  //   |  a            b  |
  // 7  \                / 4
  //   6  --------------  5
  //
  //Treat road as vector with "angle" and "len" and mA as start point
  //
  if( mProp.mStratum & stratum ) {
    double angle = mSegment.getAngleDegree();
    double len   = mSegment.getLenght();
    double halfWidth = mProp.mWidth.getValue() / 2;
    //In accordings target compares we correct width of over polygon
    if( toWhich != ruleFree ) {
      if( toWhich == ruleRoadPad || toWhich == rulePadPad )
        halfWidth += qMax( getPlate()->ruleForNet(mProp.mNetName.str(),ruleRoadPad), blk.mRules[ruleRoadPad]) + blk.mRules[ruleRoadWidth] / 2;
      else if( toWhich == ruleRoadRoad )
        halfWidth += qMax( getPlate()->ruleForNet(mProp.mNetName.str(),ruleRoadRoad),blk.mRules[ruleRoadRoad]) + blk.mRules[ruleRoadWidth] / 2;
      }
    double d = halfWidth * 0.414213562;
    QPolygonF pgn;
    pgn << QPointF( -halfWidth,       -d )         //0
        << QPointF( -d,               -halfWidth ) //1
        << QPointF(  d + len,         -halfWidth ) //2
        << QPointF(  halfWidth + len, -d )         //3
        << QPointF(  halfWidth + len,  d )         //4
        << QPointF(  d + len,          halfWidth ) //5
        << QPointF( -d,                halfWidth ) //6
        << QPointF( -halfWidth,        d );

    //Rotate polygon on vector angle
    QTransform t;
    t = t.rotate(angle);
    pgn = t.map(pgn);

    //Translate polygon to start point of vector
    pgn.translate( mSegment.getP1().x(), mSegment.getP1().y() );

    SdBarrier bar;
    bar.mNetName = mProp.mNetName.str();
    bar.mPolygon = pgn;
    dest.append( bar );
    }
  }





bool SdGraphTracedRoad::isMatchNetAndStratum(const QString netName, SdStratum stratum) const
  {
  return mProp.mNetName.str() == netName && mProp.mStratum.match( stratum );
  }




void SdGraphTracedRoad::accumWindows(SdPolyWindowList &dest, int stratum, int gap, const QString netName) const
  {
  //Test if match to stratum and not equal to netName
  if( mProp.mNetName == netName || (mProp.mStratum & stratum) == 0 )
    return;
  //We append 3 objects: over rectangle, 2 end solid filled circles

  //Make around line rectangle
  //
  //   0  --------------  1
  //     |              |
  //     |a            b|
  //     |              |
  //   3  --------------  2
  //
  //Treat road as vector with "angle" and "len" and mA as start point
  //
  double angle = mSegment.getAngleDegree();
  double len   = mSegment.getLenght();
  double halfWidth = mProp.mWidth.getValue() / 2 + gap;
  QPolygonF pgn;
  pgn << QPointF(  0,       -halfWidth )         //0
      << QPointF(  len,     -halfWidth ) //1
      << QPointF(  len,      halfWidth ) //2
      << QPointF(  0,        halfWidth );//3

  //Rotate polygon on vector angle
  QTransform t;
  t = t.rotate(angle);
  pgn = t.map(pgn);

  //Translate polygon to start point of vector
  pgn.translate( mSegment.getP1().x(), mSegment.getP1().y() );

  dest.appendRegion( pgn );
  dest.appendCircle( mSegment.getP1(), static_cast<int>(halfWidth) );
  dest.appendCircle( mSegment.getP2(), static_cast<int>(halfWidth) );
  }





//Check if road linked to point
bool SdGraphTracedRoad::isLinked(SdPoint a, SdStratum stratum, QString netName) const
  {
  return mProp.mNetName == netName && mProp.mStratum.match(stratum) && (mSegment.getP1() == a || mSegment.getP2() == a);
  }






//Return layer for road stratum
SdLayer *SdGraphTracedRoad::getLayer() const
  {
  return sdEnvir->mCacheForRoad.getVisibleLayer( mProp.mStratum );
  }




//Single road linked to point. If more than one road then no roads return
SdGraphTracedRoad *SdGraphTracedRoad::linkedRoad(SdPoint p)
  {
  SdGraphTracedRoad *road = nullptr;
  QString netName = mProp.mNetName.str();
  SdStratum st = mProp.mStratum;
  getPlate()->forEach( dctTraced, [p,&road,st,netName,this] (SdObject *obj) -> bool {
    SdPtr<SdGraphTraced> traced(obj);
    if( traced.isValid() && obj != this ) {
      //Test if point linked to traced object
      if( traced->isLinked( p, st, netName ) ) {
        //point is linked
        if( traced->getClass() != dctTraceRoad ) {
          //Any linked object other than road return "no linked"
          road = nullptr;
          return false;
          }
        else if( road ) {
          //Linked object is road. Already there is linked segment.
          //So there is at least two linked segments, so return "no linked"
          road = nullptr;
          return false;
          }
        else {
          //First road object. Fix it.
          road = dynamic_cast<SdGraphTracedRoad*>(obj);
          }
        }
      }
    return true;
    });
  return road;
  }





//Utilize on end p
void SdGraphTracedRoad::utilizeAtEnd(SdPoint p, SdUndo *undo)
  {
  SdGraphTracedRoad *road = linkedRoad( p );
  if( road && road->segment().getP1() == road->segment().getP2() ) {
    //Delete segment with zero lenght
    road->deleteObject( undo );
    return;
    }
  if( road && mProp == road->propRoad() ) {
    //Utilize segemtns only with same properties
    SdPoint d = p == road->segment().getP1() ? road->segment().getP2() : road->segment().getP1();
    if( is3PointsOnLine( mSegment.getP1(), mSegment.getP2(), d ) ) {
      //Segment can be utilized
      if( p == mSegment.getP1() )
        mSegment.setP2( d );
      else
        mSegment.setP1( d );
      }
    }
  }
