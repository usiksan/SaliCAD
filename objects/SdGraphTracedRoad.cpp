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
    mNetName = road->mNetName;
    mA       = road->mA;
    mB       = road->mB;
    mProp    = road->mProp;
    mStratum = road->mStratum;
    }
  }



void SdGraphTracedRoad::writeObject(QJsonObject &obj) const
  {
  //Write top
  SdGraphTraced::writeObject( obj );
  //Members of this class
  obj.insert( QStringLiteral("Net"), mNetName );
  mA.write( QStringLiteral("a"), obj );
  mB.write( QStringLiteral("b"), obj );
  mProp.write( obj );
  mStratum.writeStratum( obj );
  }



void SdGraphTracedRoad::readObject(SdObjectMap *map, const QJsonObject obj)
  {
  //Read top
  SdGraphTraced::readObject( map, obj );
  //Members of this class
  mNetName = obj.value( QStringLiteral("Net") ).toString();
  mA.read( QStringLiteral("a"), obj );
  mB.read( QStringLiteral("b"), obj );
  mProp.read( obj );
  mStratum.readStratum( obj );
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





void SdGraphTracedRoad::setProp(SdPropSelected &prop)
  {
  }




void SdGraphTracedRoad::getProp(SdPropSelected &prop)
  {
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
  }




bool SdGraphTracedRoad::isVisible()
  {
  //Layer of road
  SdLayer *layer = sdEnvir->mCacheForRoad.getLayer(mStratum);
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
    info = QObject::tr("Net: ") + mNetName;
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
  return false;
  }



void SdGraphTracedRoad::accumNetPoints(SdPlateNetList &netList)
  {
  }




void SdGraphTracedRoad::drawStratum(SdContext *dcx, int stratum)
  {
  if( mStratum & stratum ) {
    //Layer of road
    SdLayer *layer = sdEnvir->mCacheForRoad.getLayer(mStratum);
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
