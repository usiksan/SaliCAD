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

#include <QTransform>

SdGraphTracedRoad::SdGraphTracedRoad()
  {

  }

SdGraphTracedRoad::SdGraphTracedRoad(const SdPropRoad &prp, SdPoint a, SdPoint b) :
  SdGraphTraced(),
  mProp(prp),
  mSegment(a,b),
  mFly(flyAB)
  {

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
    mA       = road->mA;
    mB       = road->mB;
    mProp    = road->mProp;
    }
  }



void SdGraphTracedRoad::writeObject(QJsonObject &obj) const
  {
  //Write top
  SdGraphTraced::writeObject( obj );
  //Members of this class
  mA.write( QStringLiteral("a"), obj );
  mB.write( QStringLiteral("b"), obj );
  mProp.write( obj );
  }



void SdGraphTracedRoad::readObject(SdObjectMap *map, const QJsonObject obj)
  {
  //Read top
  SdGraphTraced::readObject( map, obj );
  //Members of this class
  mA.read( QStringLiteral("a"), obj );
  mB.read( QStringLiteral("b"), obj );
  mProp.read( obj );
  }




void SdGraphTracedRoad::saveState(SdUndo *undo)
  {
  }




void SdGraphTracedRoad::moveComplete(SdPoint grid, SdUndo *undo)
  {
  }




void SdGraphTracedRoad::move(SdPoint offset)
  {
  if( mFly == flyP1P2 ) mSegment.move( offset );
  else if( mFly == flyP1 ) {
    mSegment.moveP1( offset );
    switch( mSegment.orientation() ) {
      case SdSegment::sorNull :
      case SdSegment::sorAny :
        break;
      case SdSegment::sorHorizontal :
        mSegment.moveP2( SdPoint(0,offset.y()) );
        break;
      case SdSegment::sorVertical :
        mSegment.moveP2( SdPoint(offset.x(),0) );
        break;
      case SdSegment::sorSlashForward
      }

    }
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
  }




void SdGraphTracedRoad::selectByPoint(const SdPoint p, SdSelector *selector)
  {
//  SdLayer *layer = sdEnvir->mCacheForRoad.getLayer(mProp.mStratum);
//  if( layer != nullptr ) {
//    if( layer->isEdited() ) {
//      if( !getSelector() ) {
//        //Not selected yet
//        if( mSegment.isPointOn( p ) ) {
//          //Test fixing variant
//          if( mSegment.getP1() == p ) {
//            switch( mSegment.orientation() ) {
//              case SdSegment::sorNull
//              }
//            { mFixB = true; mFixA = false; } //Point B is fixed, point A is fly
//          else if( mB == p ) { mFixA = true; mFixB = false; }  //Point A is fixed, point B is fly
//          else mFixB = mFixA = false;
//          //Проверить отсутствие фиксации на плавающем сегменте

//          //Определить способ и направление излома
//          //Define vertex direction variant
//          if( mA.x() == mB.x() ) mDirX = vdX;
//          else if( mA.y() == mB.y() ) mDirX = vdY;
//          else mDirX = vdNone;
//          selector->insert( this );
//          }
//        }
//      else {
//        //Already selected
//        if( p.isOnSegment(mA,mB) ) {
//          //If one point if fixed, then check new fix condition
//          //Если одна точка фиксирована, проверяем новое условие фиксации
//          if( mFixB && mA != p ) mFixB = false;
//          if( mFixA && mB != p ) mFixA = false;
//          }
//        }
//      }

//    }
  }

void SdGraphTracedRoad::selectByRect(const SdRect &r, SdSelector *selector)
  {
  }



void SdGraphTracedRoad::select(SdSelector *selector)
  {
  if( selector != nullptr )
    selector->insert( this );
  }



void SdGraphTracedRoad::prepareMove(SdUndo *undo)
  {
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
  return SdRect(mA, mB);
  }



int SdGraphTracedRoad::behindCursor(SdPoint p)
  {
  //Определить состояние объекта под курсором
  if( isVisible() && SdSegment(mA,mB).isPointOn( p ) )
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



bool SdGraphTracedRoad::snapPoint(SdSnapInfo *snap)
  {
  return false;
  }


bool SdGraphTracedRoad::isPointOnNet(SdPoint p, SdStratum stratum, QString *wireName, int *destStratum)
  {
  if( mProp.mStratum.match( stratum ) && SdSegment(mA,mB).isPointOn( p ) ) {
    *destStratum = mProp.mStratum.getValue();
    *wireName = mProp.mNetName.str();
    return true;
    }
  return false;
  }







void SdGraphTracedRoad::accumNetSegments(SdPlateNetList &netList) const
  {
  netList.addNetSegment( mProp.mNetName.str(), mProp.mStratum, mA, mB );
  }





void SdGraphTracedRoad::drawStratum(SdContext *dcx, int stratum)
  {
  if( mProp.mStratum & stratum ) {
    //Layer of road
    SdLayer *layer = getLayer();
    if( layer != nullptr && layer->isVisible() ) {
      dcx->setPen( mProp.mWidth.getValue(), layer, dltSolid );
      dcx->line( mA, mB );
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
    double angle = mB.getAngleDegree(mA);
    double len   = mA.getDistance(mB);
    double halfWidth = mProp.mWidth.getValue() / 2;
    //In accordings target compares we correct width of over polygon
    if( toWhich == ruleRoadPad || toWhich == rulePadPad )
      halfWidth += blk.mRules[ruleRoadPad] + blk.mRules[ruleRoadWidth] / 2;
    else if( toWhich == ruleRoadRoad )
      halfWidth += blk.mRules[ruleRoadRoad] + blk.mRules[ruleRoadWidth] / 2;
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
    pgn.translate( mA.x(), mA.y() );

    SdBarrier bar;
    bar.mNetName = mProp.mNetName.str();
    bar.mPolygon = pgn;
    dest.append( bar );
    }
  }




//Return layer for road stratum
SdLayer *SdGraphTracedRoad::getLayer() const
  {
  return sdEnvir->mCacheForRoad.getLayer( mProp.mStratum );
  }
