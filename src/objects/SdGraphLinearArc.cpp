/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Graphical object Arc.
*/
#include "SdGraphLinearArc.h"
#include "SdContext.h"
#include "SdSelector.h"
#include "SdRect.h"
#include "SdSnapInfo.h"

SdGraphLinearArc::SdGraphLinearArc() :
  SdGraphLinear()
  {

  }

SdGraphLinearArc::SdGraphLinearArc(SdPoint center, SdPoint start, SdPoint stop, const SdPropLine &propLine) :
  SdGraphLinear( propLine ),
  mCenter(center),
  mStart(start),
  mStop(stop)
  {

  }




void SdGraphLinearArc::cloneFrom(const SdObject *src)
  {
  SdGraphLinear::cloneFrom( src );
  const SdGraphLinearArc *arc = dynamic_cast<const SdGraphLinearArc*>(src);
  mCenter  = arc->mCenter;
  mStart   = arc->mStart;
  mStop    = arc->mStop;
  }




void SdGraphLinearArc::writeObject(QJsonObject &obj) const
  {
  SdGraphLinear::writeObject( obj );
  mCenter.write( QStringLiteral("Center"), obj );
  mStart.write( QStringLiteral("Start"), obj );
  mStop.write( QStringLiteral("Stop"), obj );
  }




void SdGraphLinearArc::readObject(SdObjectMap *map, const QJsonObject obj)
  {
  SdGraphLinear::readObject( map, obj );
  mCenter.read( QStringLiteral("Center"), obj );
  mStart.read( QStringLiteral("Start"), obj );
  mStop.read( QStringLiteral("Stop"), obj );
  }




void SdGraphLinearArc::saveState(SdUndo *undo)
  {
  undo->propLineAnd3Point( &mProp, &mCenter, &mStart, &mStop );
  }




void SdGraphLinearArc::move(SdPoint offset)
  {
  mCenter.move( offset );
  mStart.move( offset );
  mStop.move( offset );
  }




void SdGraphLinearArc::rotate(SdPoint center, SdPropAngle angle)
  {
  mCenter.rotate( center, angle );
  mStart.rotate( center, angle );
  mStop.rotate( center, angle );
  }




void SdGraphLinearArc::mirror(SdPoint a, SdPoint b)
  {
  mCenter.mirror( a, b );
  mStart.mirror( a, b );
  mStop.mirror( a, b );
  mStart.swap( &mStop );
  }



void SdGraphLinearArc::selectByPoint(const SdPoint p, SdSelector *selector)
  {
  if( mProp.mLayer.isEdited() ) {
    if( !getSelector() && (p == mStart || p == mStop) )
      selector->insert( this );
    }
  }



void SdGraphLinearArc::selectByRect(const SdRect &r, SdSelector *selector)
  {
  if( mProp.mLayer.isEdited() ) {
    if( !getSelector() && (r.isPointInside( mStart ) || r.isPointInside( mStop )) )
      selector->insert( this );
    }
  }





SdRect SdGraphLinearArc::getOverRect() const
  {
  return SdRect(mStart,mStop);
  }



void SdGraphLinearArc::draw(SdContext *dc)
  {
  dc->arc( mCenter, mStart, mStop, mProp );
  }




int SdGraphLinearArc::behindCursor(SdPoint p)
  {
  if( isVisible() ) {
    if( p == mStart || p == mStop ) {
      return getSelector() ? ELEM_SEL : ELEM_UNSEL;
      }
    }
  return 0;
  }



//Find snap point on object
void SdGraphLinearArc::snapPoint(SdSnapInfo *snap)
  {
  if( isVisible() ) {
    if( snap->match(snapEndPoint) ) {
      snap->test( this, mStart, snapEndPoint );
      snap->test( this, mStop, snapEndPoint );
      }
    }
  }
