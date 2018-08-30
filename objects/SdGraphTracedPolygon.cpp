/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Graphical traced polygon object. Traced object is object which represents printed polygin with windows.
*/
#include "SdGraphTracedPolygon.h"
#include "SdContext.h"
#include "SdEnvir.h"
#include "SdSelector.h"

SdGraphTracedPolygon::SdGraphTracedPolygon()
  {

  }




SdGraphTracedPolygon::SdGraphTracedPolygon(const SdPropPolygon &prp, SdPointList lst, SdPolyWindowList win) :
  SdGraphTraced(),
  mProp(prp),
  mRegion(lst),
  mWindows(win)
  {

  }





QString SdGraphTracedPolygon::getType() const
  {
  return QStringLiteral(SD_TYPE_GRAPH_TRACE_POLYGON);
  }




SdClass SdGraphTracedPolygon::getClass() const
  {
  return dctTracePolygon;
  }




void SdGraphTracedPolygon::attach(SdUndo *undo)
  {
  }

void SdGraphTracedPolygon::detach(SdUndo *undo)
  {
  }




void SdGraphTracedPolygon::cloneFrom(const SdObject *src)
  {
  //Clone top
  SdGraphTraced::cloneFrom(src);
  //Clone current
  SdPtrConst<SdGraphTracedPolygon> poly(src);
  if( poly.isValid() ) {
    mProp      = poly->mProp;
    mRegion    = poly->mRegion;
    mWindows   = poly->mWindows;
    }
  }





void SdGraphTracedPolygon::writeObject(QJsonObject &obj) const
  {
  //Write top
  SdGraphTraced::writeObject( obj );
  //Members of this class
  mProp.write( obj );
  mRegion.write( QStringLiteral("region"), obj );
  mWindows.write( QStringLiteral("windows"), obj );
  }




void SdGraphTracedPolygon::readObject(SdObjectMap *map, const QJsonObject obj)
  {
  //Read top
  SdGraphTraced::readObject( map, obj );
  //Members of this class
  mProp.read( obj );
  mRegion.read( QStringLiteral("region"), obj );
  mWindows.read( QStringLiteral("windows"), obj );
  }




void SdGraphTracedPolygon::saveState(SdUndo *undo)
  {
  }

void SdGraphTracedPolygon::moveComplete(SdPoint grid, SdUndo *undo)
  {
  }

void SdGraphTracedPolygon::move(SdPoint offset)
  {
  }

void SdGraphTracedPolygon::rotate(SdPoint center, SdPropAngle angle)
  {
  }

void SdGraphTracedPolygon::setProp(SdPropSelected &prop)
  {
  }

void SdGraphTracedPolygon::getProp(SdPropSelected &prop)
  {
  }

void SdGraphTracedPolygon::selectByPoint(const SdPoint p, SdSelector *selector)
  {
  }




void SdGraphTracedPolygon::selectByRect(const SdRect &r, SdSelector *selector)
  {
//  SdLayer *layer = getLayer();
//  if( layer != nullptr && layer->isEdited() ) {
//    if( !getSelector() ) {
//      //Not selected yet
//      if( r.isAccross( mSegment ) ) {
//        if( r.isPointInside(mSegment.getP1()) && !r.isPointInside(mSegment.getP2()) ) {
//          if( mSegment.orientation() == SdSegment::sorAny ) mFly = flyP1;
//          else mFly = flyP1_45;
//          }
//        else if( r.isPointInside(mSegment.getP2()) && !r.isPointInside(mSegment.getP1()) ) {
//          if( mSegment.orientation() == SdSegment::sorAny ) mFly = flyP2;
//          else mFly = flyP2_45;
//          }
//        else mFly = flyP1P2;
//        selector->insert( this );
//        }
//      }
//    else {
//      //Already selected
//      if( r.isAccross( mSegment ) ) {
//        //If inside selecting rect then check if was only one end selected then select both
//        if( r.isPointInside(mSegment.getP1()) && (mFly == flyP2 || mFly == flyP2_45) ) mFly = flyP1P2;
//        else if( r.isPointInside(mSegment.getP2()) && (mFly == flyP1 || mFly == flyP1_45) ) mFly = flyP1P2;
//        }
//      }
//    }
  }





void SdGraphTracedPolygon::select(SdSelector *selector)
  {
  if( selector != nullptr )
    selector->insert( this );
  }




void SdGraphTracedPolygon::setLayerUsage()
  {
  //Layer of polygon
  SdLayer *layer = getLayer();
  if( layer != nullptr )
    layer->setUsage();
  }




bool SdGraphTracedPolygon::isVisible()
  {
  //Layer of polygon
  SdLayer *layer = getLayer();
  return layer != nullptr && layer->isVisible();
  }




SdRect SdGraphTracedPolygon::getOverRect() const
  {
  return mRegion.boundingRect();
  }




void SdGraphTracedPolygon::draw(SdContext *dc)
  {
  dc->polygon( mRegion, mWindows, getLayer() );
  }




int SdGraphTracedPolygon::behindCursor(SdPoint p)
  {

  }



bool SdGraphTracedPolygon::getInfo(SdPoint p, QString &info, bool extInfo)
  {
  }

bool SdGraphTracedPolygon::snapPoint(SdSnapInfo *snap)
  {
  }

SdStratum SdGraphTracedPolygon::stratum() const
  {
  }




bool SdGraphTracedPolygon::isPointOnNet(SdPoint p, SdStratum stratum, QString *wireName, int *destStratum)
  {
  }




void SdGraphTracedPolygon::accumNetSegments(SdPlateNetList &netList) const
  {
  }




void SdGraphTracedPolygon::drawStratum(SdContext *dcx, int stratum)
  {
  if( mProp.mStratum & stratum ) {
    //Layer of road
    SdLayer *layer = getLayer();
    if( layer != nullptr && layer->isVisible() )
      dcx->polygon( mRegion, mWindows, getLayer() );
    }
  }




void SdGraphTracedPolygon::accumBarriers(SdBarrierList &dest, int stratum, SdRuleId toWhich, const SdRuleBlock &blk) const
  {
  }




bool SdGraphTracedPolygon::isMatchNetAndStratum(const QString netName, SdStratum stratum) const
  {
  return mProp.mNetName.str() == netName && mProp.mStratum.match( stratum );
  }



void SdGraphTracedPolygon::accumWindows(SdPolyWindowList &dest, int stratum, int gap, const QString netName) const
  {

  }




//Return layer for polygon stratum
SdLayer *SdGraphTracedPolygon::getLayer() const
  {
  return sdEnvir->mCacheForPolygon.getLayer( mProp.mStratum );
  }
