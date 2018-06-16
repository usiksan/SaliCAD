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
#include <QTransform>

SdGraphTracedRoad::SdGraphTracedRoad()
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
  }

void SdGraphTracedRoad::selectByRect(const SdRect &r, SdSelector *selector)
  {
  }

void SdGraphTracedRoad::select(SdSelector *selector)
  {
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




bool SdGraphTracedRoad::isPointOnNet(SdPoint p, SdStratum stratum, QString &wireName)
  {
  if( mProp.mStratum.match( stratum ) && SdSegment(mA,mB).isPointOn( p ) ) {
    wireName = mProp.mNetName.str();
    return true;
    }
  return false;
  }



void SdGraphTracedRoad::accumNetPoints(SdPlateNetList &netList)
  {
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
  if( mStratum & stratum ) {
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
    bar.mNetName = mNetName;
    bar.mPolygon = pgn;
    dest.append( bar );
    }
  }




//Return layer for road stratum
SdLayer *SdGraphTracedRoad::getLayer() const
  {
  return sdEnvir->mCacheForRoad.getLayer( mProp.mStratum );
  }
