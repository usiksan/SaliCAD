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

SdGraphTracedPolygon::SdGraphTracedPolygon()
  {

  }




SdGraphTracedPolygon::SdGraphTracedPolygon(const SdPropRoad &prp, SdPointList lst, SdPolyWindowList win) :
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
  }

void SdGraphTracedPolygon::select(SdSelector *selector)
  {
  }

void SdGraphTracedPolygon::setLayerUsage()
  {
  }

bool SdGraphTracedPolygon::isVisible()
  {
  }

SdRect SdGraphTracedPolygon::getOverRect() const
  {
  }

void SdGraphTracedPolygon::draw(SdContext *dc)
  {
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
  }

void SdGraphTracedPolygon::accumBarriers(SdBarrierList &dest, int stratum, SdRuleId toWhich, const SdRuleBlock &blk) const
  {
  }

bool SdGraphTracedPolygon::isMatchNetAndStratum(const QString netName, SdStratum stratum) const
  {
  }
