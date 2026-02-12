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
#include "SdPItemPlate.h"
#include "SdPlateNetContainer.h"
#include "SdJsonIO.h"

#include <QDebug>

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
  Q_UNUSED(undo);
  rebuildWindows();
  }




//!
//! \brief cloneFrom Overrided function. We copy object from source
//! \param src       Source of object from which copy must be made
//! \param copyMap   Structure for mapping copying substitutes
//! \param next      Make simple or next copy. Next copy available not for all objects.
//!                  For example: pin name A23 with next copy return A24
//!
void SdGraphTracedPolygon::cloneFrom(const SdObject *src, SdCopyMap &copyMap, bool next)
  {
  //Clone top
  SdGraphTraced::cloneFrom( src, copyMap, next );
  //Clone current
  SdPtrConst<SdGraphTracedPolygon> poly(src);
  if( poly.isValid() ) {
    mProp      = poly->mProp;
    mRegion    = poly->mRegion;
    mWindows   = poly->mWindows;
    }
  }





//!
//! \brief json Overloaded function to write object content into json writer
//!             Overrided function
//! \param js   Json writer
//!
void SdGraphTracedPolygon::json(SdJsonWriter &js) const
  {
  //Members of this class
  mProp.json( js );
  mRegion.json( QStringLiteral("region"), js );
  js.jsonList( js, QStringLiteral("windows"), mWindows );
  //Write top
  SdGraphTraced::json( js );
  }




//!
//! \brief json Overloaded function to read object content from json reader
//!             Overrided function
//! \param js   Json reader
//!
void SdGraphTracedPolygon::json(const SdJsonReader &js)
  {
  //Members of this class
  mProp.json( js );
  mRegion.json( QStringLiteral("region"), js );
  js.jsonList( js, QStringLiteral("windows"), mWindows );
  //Write top
  SdGraphTraced::json( js );
  }







//Save state of polygon
void SdGraphTracedPolygon::saveState(SdUndo *undo)
  {
  if( undo )
    undo->polygon( &mProp, &mRegion, &mWindows );
  }




void SdGraphTracedPolygon::moveComplete(SdPoint grid, SdUndo *undo)
  {
  Q_UNUSED(grid)
  Q_UNUSED(undo)
  //Rebuild window list
  rebuildWindows();
  }




void SdGraphTracedPolygon::move(SdPoint offset)
  {
  mRegion.move( mFlyIndex, offset );
  }




void SdGraphTracedPolygon::rotate(SdPoint center, SdPropAngle angle)
  {
  mRegion.rotate( center, angle );
  }




//Properties service [Изменение свойствами]
//Set properties of this object from prop
void SdGraphTracedPolygon::setProp(SdPropSelected &prop)
  {
  //From global we can set only gap
  mProp.mGap = prop.mPolygonProp.mGap;
  }





//Get (append) properties from this object to prop
void SdGraphTracedPolygon::getProp(SdPropSelected &prop)
  {
  prop.mPolygonProp.append( mProp );
  prop.mFilledPropMask |= spsPolygonProp;
  }






void SdGraphTracedPolygon::selectByPoint(const SdPoint p, SdSelector *selector)
  {
  SdLayer *layer = getLayer();
  if( layer != nullptr && layer->isEdited() ) {
    //For each pair points of list check segment made by this pair
    //and if test point p is on segment, then append two point of pair to select list
    for( int i = 0; i < mRegion.count() - 1; ++i )
      if( p.isOnSegment( mRegion.get(i), mRegion.get(i+1)) ) {
        if( !getSelector() ) {
          mFlyIndex.clear();
          selector->insert( this );
          }
        if( mRegion.get(i) == p ) mFlyIndex.insert( i );
        else if( mRegion.get(i+1) == p ) mFlyIndex.insert( i + 1 );
        else {
          mFlyIndex.insert( i );
          mFlyIndex.insert( i + 1 );
          }
        }
    }
  }




void SdGraphTracedPolygon::selectByRect(const SdRect &r, SdSelector *selector)
  {
  //Ploygon selected if any vertex of polygon foll inside rectangle
  SdLayer *layer = getLayer();
  if( layer != nullptr && layer->isEdited() ) {
    for( int i = 0; i < mRegion.count() - 1; ++i )
      if( r.isAccross( mRegion.get(i), mRegion.get(i+1)) ) {
        if( !getSelector() ) {
          mFlyIndex.clear();
          selector->insert( this );
          }
        mFlyIndex.insert( i );
        mFlyIndex.insert( i + 1 );
        }
    }
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






int SdGraphTracedPolygon::behindCursor(SdPoint p)
  {
  //Check if object is behind cursor
  //Определить состояние объекта под курсором
  if( isVisible() ) {
    //For each segments of region test if point is on segment
    for( int i = 0; i < mRegion.count()-1; ++i )
      if( p.isOnSegment( mRegion.get(i), mRegion.get(i+1)) )
        return getSelector() ? ELEM_SEL : ELEM_UNSEL;
    //And at end test last (closing) segment
    if( p.isOnSegment( mRegion.last(), mRegion.first() ) )
      return getSelector() ? ELEM_SEL : ELEM_UNSEL;
    }
  return 0;
  }



bool SdGraphTracedPolygon::getInfo(SdPoint p, QString &info, bool extInfo)
  {
  Q_UNUSED(extInfo)
  if( behindCursor( p ) ) {
    info = QObject::tr("Net: ") + mProp.mNetName.str();
    return true;
    }
  return false;
  }



//Find snap point on object
void SdGraphTracedPolygon::snapPoint(SdSnapInfo *snap)
  {
  if( snap->mStratum.match( mProp.mStratum ) && !snap->match(snapExcludeSour) ) {
//    if( snap->match(snapNearestNet) ) {
//      if( isContains(snap->mSour) ) {
//        snap->test( this, snap->mSour, snapNearestNet );
//        }
//      }
    if( snap->match(snapNearestNetNet) && snap->mNetName == mProp.mNetName.str() ) {
      if( isContains(snap->mSour) ) {
        snap->test( this, snap->mSour, snapNearestNetNet );
        }
      }
    }
  }




SdStratum SdGraphTracedPolygon::stratum() const
  {
  return mProp.mStratum;
  }




bool SdGraphTracedPolygon::isPointOnNet(SdPoint p, SdStratum stratum, QString *netName, int *destStratum)
  {
  if( mProp.mStratum & stratum ) {
    if( isContains(p) ) {
      if( *netName == mProp.mNetName.str() )
        *destStratum |= mProp.mStratum.getValue();
      else {
        *destStratum = mProp.mStratum.getValue();
        *netName = mProp.mNetName.str();
        }
      return true;
      }
    }
  return false;
  }






void SdGraphTracedPolygon::accumNetSegments(SdPlateNetContainer *netContainer)
  {
//  if( mRegion.count() )
//    netContainer->addNetSegment( this, mProp.mNetName.str(), mProp.mStratum, mRegion.at(0), mRegion.at(0) );
  //TODO B049 net segments creation for polygon
  //Idea: we scan all traced object and append for each object to polygon connection segment mRegion[0]-object
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
  //TODO B048 barriars creation for polygon
  }




bool SdGraphTracedPolygon::isMatchNetAndStratum(const QString netName, SdStratum stratum) const
  {
  return mProp.mNetName.str() == netName && mProp.mStratum.match( stratum );
  }





void SdGraphTracedPolygon::accumWindows(SdPolyWindowList &dest, int stratum, int gap, const QString netName) const
  {
  if( (mProp.mStratum & stratum) && mProp.mNetName.str() != netName ) {
    //Find polygon intersection
    QPolygon pgn = dest.polygon()->intersected( mRegion );

    //Append window only if there intersection
    if( pgn.count() ) {
      //Calc over rectangle
      SdRect r = pgn.boundingRect();

      //Append rect window
      dest.append( SdPolyWindow( r, gap ) );
      }
    }
  }






//Check if polygon linked to point
bool SdGraphTracedPolygon::isLinked(SdPoint a, SdStratum stratum, QString netName) const
  {
  return mProp.mNetName == netName && mProp.mStratum.match( stratum ) && isContains(a);
  }







//Return layer for polygon stratum
SdLayer *SdGraphTracedPolygon::getLayer() const
  {
  return SdEnvir::instance()->layerVisibleForPolygon( mProp.mStratum );
  }




//Rebuild windows list
void SdGraphTracedPolygon::rebuildWindows()
  {
  //Clear current windows list
  mWindows.reset( &mRegion );

  //Accum on plate windows
  getParent()->forEach( dctTraced, [this] (SdObject *obj) -> bool {
    SdPtr<SdGraphTraced> traced( obj );
    if( traced.isValid() )
      traced->accumWindows( mWindows, mProp.mStratum.stratum(), mProp.mGap.getValue(), mProp.mNetName.str() );
    return true;
    });

  }




//Return true if point is on polygon
bool SdGraphTracedPolygon::isContains(SdPoint p) const
  {
  return mRegion.containsPoint( p, Qt::OddEvenFill ) && !mWindows.containsPoint( p );
  }
