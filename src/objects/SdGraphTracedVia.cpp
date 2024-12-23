#include "SdGraphTracedVia.h"
#include "SdPItemPlate.h"
#include "SdPlateNetList.h"
#include "SdSnapInfo.h"
#include "SdUndo.h"
#include "SdJsonIO.h"

#include <QTransform>

SdGraphTracedVia::SdGraphTracedVia()
  {

  }

SdGraphTracedVia::SdGraphTracedVia(SdPoint pos, const SdPropVia &prp) :
  SdGraphTraced(),
  mPosition(pos),
  mProp(prp)
  {

  }




//SdGraphTracedVia::SdGraphTracedVia(const QString netName, SdStratum stratum, const QString padType, SdPoint pos)
//  {

//  }




QString SdGraphTracedVia::getType() const
  {
  return QStringLiteral(SD_TYPE_GRAPH_TRACE_VIA);
  }




SdClass SdGraphTracedVia::getClass() const
  {
  return dctTraceVia;
  }






//!
//! \brief cloneFrom Overrided function. We copy object from source
//! \param src       Source of object from which copy must be made
//! \param copyMap   Structure for mapping copying substitutes
//! \param next      Make simple or next copy. Next copy available not for all objects.
//!                  For example: pin name A23 with next copy return A24
//!
void SdGraphTracedVia::cloneFrom(const SdObject *src, SdCopyMap &copyMap, bool next)
  {
  //Clone top
  SdGraphTraced::cloneFrom(src, copyMap, next);
  //Clone current
  SdPtrConst<SdGraphTracedVia> via(src);
  if( via.isValid() ) {
    mPosition = via->mPosition;
    mProp     = via->mProp;
    }
  }




//!
//! \brief json Overloaded function to write object content into json writer
//!             Overrided function
//! \param js   Json writer
//!
void SdGraphTracedVia::json(SdJsonWriter &js) const
  {
  //Members of this class
  js.jsonPoint( QStringLiteral("pos"), mPosition );
  mProp.json( js );
  //Top
  SdGraphTraced::json( js );
  }




//!
//! \brief json Overloaded function to read object content from json reader
//!             Overrided function
//! \param js   Json reader
//!
void SdGraphTracedVia::json(const SdJsonReader &js)
  {
  //Members of this class
  js.jsonPoint( QStringLiteral("pos"), mPosition );
  mProp.json( js );
  //Top
  SdGraphTraced::json( js );
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
    return getSelector() ? ELEM_SEL : ELEM_UNSEL;
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




//Find snap point on object
void SdGraphTracedVia::snapPoint(SdSnapInfo *snap)
  {
  if( snap->match( snapNearestPin | snapNearestNetPin ) ) {
    //Perform snap
    if( (snap->match( snapNearestPin ) || snap->mNetName == mProp.mNetName.str()) && snap->mStratum.match(mProp.mStratum) )
      snap->test( this, mPosition, snapNearestPin | snapNearestNetPin );
    }
  if( snap->match( snapNearestNetVia ) )
    snap->test( this, mPosition, snapNearestNetVia );
  if( snap->match( snapViaPoint )  )
    snap->test( this, mPosition, snapViaPoint );
  }





SdStratum SdGraphTracedVia::stratum() const
  {
  return mProp.mStratum;
  }





bool SdGraphTracedVia::isPointOnNet(SdPoint p, SdStratum stratum, QString *netName, int *destStratum)
  {
  if( mPosition == p && mProp.mStratum.match( stratum ) ) {
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






void SdGraphTracedVia::accumNetSegments(SdPlateNetContainer *netContainer)
  {
  netContainer->addNetSegment( this, mProp.mNetName.str(), mProp.mStratum, mPosition, mPosition );
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
    switch( toWhich ) {
      default:
      case ruleFree :
      case ruleRoadWidth :
        //Only pure pad without any clearance
        clearance = halfWidth = 0;
        break;
      case rulePadPad :
        clearance = qMax( plate->ruleForNet(mProp.mNetName.str(),rulePadPad), blk.mRules[rulePadPad] );
        halfWidth = blk.mRules[ruleRoadWidth] / 2;
        break;
      case ruleRoadPad :
      case ruleRoadRoad :
        clearance = qMax( plate->ruleForNet(mProp.mNetName.str(),ruleRoadPad), blk.mRules[ruleRoadPad] );
        halfWidth = blk.mRules[ruleRoadWidth] / 2;
        break;
      }
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




//Check if road linked to point
bool SdGraphTracedVia::isLinked(SdPoint a, SdStratum stratum, QString netName) const
  {
  return mProp.mNetName == netName && mProp.mStratum.match( stratum ) && mPosition == a;
  }





