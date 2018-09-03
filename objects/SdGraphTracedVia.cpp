#include "SdGraphTracedVia.h"
#include "SdPItemPlate.h"
#include "SdPlateNetList.h"
#include "SdSnapInfo.h"
#include "SdUndo.h"

#include <QTransform>

SdGraphTracedVia::SdGraphTracedVia()
  {

  }




QString SdGraphTracedVia::getType() const
  {
  return QStringLiteral(SD_TYPE_GRAPH_TRACE_VIA);
  }




SdClass SdGraphTracedVia::getClass() const
  {
  return dctTraceVia;
  }




void SdGraphTracedVia::cloneFrom(const SdObject *src)
  {
  //Clone top
  SdGraphTraced::cloneFrom(src);
  //Clone current
  SdPtrConst<SdGraphTracedVia> via(src);
  if( via.isValid() ) {
    mPosition = via->mPosition;
    mProp     = via->mProp;
    }
  }




void SdGraphTracedVia::writeObject(QJsonObject &obj) const
  {
  //Write top
  SdGraphTraced::writeObject( obj );
  //Members of this class
  mPosition.write( QStringLiteral("pos"), obj );
  mProp.write( obj );
  }




void SdGraphTracedVia::readObject(SdObjectMap *map, const QJsonObject obj)
  {
  //Read top
  SdGraphTraced::readObject( map, obj );
  //Members of this class
  mPosition.read( QStringLiteral("pos"), obj );
  mProp.read( obj );
  }




void SdGraphTracedVia::saveState(SdUndo *undo)
  {
  undo->via( &(mProp.mPadType), &mPosition );
  }




void SdGraphTracedVia::moveComplete(SdPoint grid, SdUndo *undo)
  {
  Q_UNUSED(grid)
  Q_UNUSED(undo)
  }




void SdGraphTracedVia::move(SdPoint offset)
  {
  mPosition.move( offset );
  }




//Properties service [Изменение свойствами]
//Set properties of this object from prop
void SdGraphTracedVia::setProp(SdPropSelected &prop)
  {
  //From global we can set only pad type
  mProp.mPadType = prop.mViaProp.mPadType;
  }




void SdGraphTracedVia::getProp(SdPropSelected &prop)
  {
  prop.mViaProp.append( mProp );
  prop.mFilledPropMask |= spsRoadProp;
  }





void SdGraphTracedVia::selectByPoint(const SdPoint p, SdSelector *selector)
  {
  if( !getSelector() && p == mPosition )
    selector->insert( this );
  }




void SdGraphTracedVia::selectByRect(const SdRect &r, SdSelector *selector)
  {
  if( !getSelector() && r.isPointInside(mPosition) )
    selector->insert( this );
  }




void SdGraphTracedVia::select(SdSelector *selector)
  {
  if( selector != nullptr )
    selector->insert( this );
  }




void SdGraphTracedVia::prepareMove(SdUndo *undo)
  {
  Q_UNUSED(undo);
  }




void SdGraphTracedVia::setLayerUsage()
  {
  }




bool SdGraphTracedVia::isVisible()
  {
  return true;
  }





SdRect SdGraphTracedVia::getOverRect() const
  {
  return getPlate()->getPadPolygon( mPosition, mProp.mPadType.str(), 0 ).boundingRect().toRect();
  }





int SdGraphTracedVia::behindCursor(SdPoint p)
  {
  if( mPosition == p )
    return getSelector() ? SEL_ELEM : UNSEL_ELEM;
  return 0;
  }





bool SdGraphTracedVia::getInfo(SdPoint p, QString &info, bool extInfo)
  {
  Q_UNUSED(extInfo)
  if( behindCursor( p ) ) {
    info = QObject::tr("Net: ") + mProp.mNetName.str();
    return true;
    }
  return false;
  }




bool SdGraphTracedVia::snapPoint(SdSnapInfo *snap)
  {
  if( snap->match( snapNearestPin | snapNearestNetPin ) ) {
    //Perform snap
    if( (snap->match( snapNearestPin ) || snap->mNetName == mProp.mNetName.str()) && snap->mStratum.match(mProp.mStratum) )
      return snap->test( mPosition, snapNearestPin | snapNearestNetPin );
    }
  return false;
  }





SdStratum SdGraphTracedVia::stratum() const
  {
  return mProp.mStratum;
  }





bool SdGraphTracedVia::isPointOnNet(SdPoint p, SdStratum stratum, QString *wireName, int *destStratum)
  {
  if( mPosition == p && mProp.mStratum.match( stratum ) ) {
    *destStratum = mProp.mStratum.getValue();
    *wireName = mProp.mNetName.str();
    return true;
    }
  return false;
  }





void SdGraphTracedVia::accumNetSegments(SdPlateNetList &netList) const
  {
  netList.addNetSegment( mProp.mNetName.str(), mProp.mStratum, mPosition, mPosition );
  }





void SdGraphTracedVia::drawStratum(SdContext *dcx, int stratum)
  {
  //Draw pin pad
  getPlate()->drawPad( dcx, mPosition, mProp.mPadType.str(), mProp.mStratum.stratum() & stratum );
  }





void SdGraphTracedVia::accumBarriers(SdBarrierList &dest, int stratum, SdRuleId toWhich, const SdRuleBlock &blk) const
  {
  //Compare on stratum
  if( mProp.mStratum & stratum ) {
    //Accum barriers for all pin pad
    SdPItemPlate *plate = getPlate();
    //Addon clearance and halfWidth
    //Clearance needed to determine which clearance is max one and use
    //halfWidth needed to trace road as line with null width because absent intersection
    //allow trace road
    int clearance, halfWidth;
    if( toWhich == ruleRoadWidth )
      clearance = halfWidth = 0;
    else {
      if( toWhich == ruleRoadRoad ) toWhich = ruleRoadPad;
      clearance = blk.mRules[toWhich];
      halfWidth = blk.mRules[ruleRoadWidth] / 2;
      }

    if( toWhich >= ruleLast )
      clearance = 0;
    else
      clearance = qMax( clearance, plate->ruleForNet( stratum, mProp.mNetName.str(), toWhich ) );
    SdBarrier bar;
    bar.mNetName = mProp.mNetName.str();
    bar.mPolygon = plate->getPadPolygon( mPosition, mProp.mPadType.str(), clearance + halfWidth );
    dest.append( bar );
    }
  }





bool SdGraphTracedVia::isMatchNetAndStratum(const QString netName, SdStratum stratum) const
  {
  return mProp.mNetName.str() == netName && mProp.mStratum.match( stratum );
  }





void SdGraphTracedVia::accumWindows(SdPolyWindowList &dest, int stratum, int gap, const QString netName) const
  {
  //Compare on stratum
  if( (mProp.mStratum & stratum) && mProp.mNetName.str() != netName ) {
    QTransform t;
    //Stratum matched and net name other
    getPlate()->appendPadWindow( dest, mPosition, mProp.mPadType.str(), gap, t );
    }
  }

