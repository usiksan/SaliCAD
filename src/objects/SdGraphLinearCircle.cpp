/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Graphical object Circle.
*/
#include "SdGraphLinearCircle.h"
#include "SdContext.h"
#include "SdSelector.h"
#include "SdRect.h"
#include "SdSnapInfo.h"
#include "Sd3dDraw.h"

SdGraphLinearCircle::SdGraphLinearCircle() :
  SdGraphLinear(),
  mCenter(),
  mRadius(0)
  {

  }




SdGraphLinearCircle::SdGraphLinearCircle(SdPoint center, int radius, const SdPropLine &propLine) :
  SdGraphLinear( propLine ),
  mCenter( center ),
  mRadius( radius )
  {

  }




void SdGraphLinearCircle::cloneFrom(const SdObject *src)
  {
  SdGraphLinear::cloneFrom( src );
  const SdGraphLinearCircle *circle = dynamic_cast<const SdGraphLinearCircle*>(src);
  if( circle ) {
    mCenter = circle->mCenter;
    mRadius = circle->mRadius;
    }
  }




void SdGraphLinearCircle::writeObject(QJsonObject &obj) const
  {
  SdGraphLinear::writeObject( obj );
  mCenter.write( QString("center"), obj );
  obj.insert( QString("radius"), mRadius );
  }




void SdGraphLinearCircle::readObject(SdObjectMap *map, const QJsonObject obj)
  {
  SdGraphLinear::readObject( map, obj );
  mCenter.read( QString("center"), obj );
  mRadius = obj.value( QString("radius") ).toInt();
  }




void SdGraphLinearCircle::move(SdPoint offset)
  {
  mCenter.move( offset );
  }




void SdGraphLinearCircle::rotate(SdPoint center, SdPropAngle angle)
  {
  mCenter.rotate( center, angle );
  }




void SdGraphLinearCircle::mirror(SdPoint a, SdPoint b)
  {
  mCenter.mirror( a, b );
  }




void SdGraphLinearCircle::selectByPoint(const SdPoint p, SdSelector *selector)
  {
  if( mProp.mLayer.isEdited() ) {
    if( !getSelector() && p.isOnCircle( mCenter, mRadius ) )
      selector->insert( this );
    }
  }




void SdGraphLinearCircle::selectByRect(const SdRect &r, SdSelector *selector)
  {
  if( mProp.mLayer.isEdited() ) {
    SdRect cr( mCenter.x() - mRadius, mCenter.y() - mRadius, 2 * mRadius, 2 * mRadius );
    if( !getSelector() && r.isAccross(cr) )
      selector->insert( this );
    }
  }





SdRect SdGraphLinearCircle::getOverRect() const
  {
  SdRect cr( mCenter.x() - mRadius, mCenter.y() - mRadius, 2 * mRadius, 2 * mRadius );
  return cr;
  }




void SdGraphLinearCircle::draw(SdContext *dc)
  {
  dc->circle( mCenter, mRadius, mProp );
  }






//!
//! \brief draw3d Draws object in 3d space
//! \param f      3d draw functions with predefined 3d context
//!
void SdGraphLinearCircle::draw3d(QOpenGLFunctions_2_0 *f) const
  {
  Sd3dDraw::circle( f, mCenter, mRadius, -0.1 );
  }




//Find snap point on object
void SdGraphLinearCircle::snapPoint(SdSnapInfo *snap)
  {
  if( isVisible() ) {
    if( snap->match( snapCenter ) )
      snap->test( this, mCenter, snapCenter );

    if( snap->match( snapQuadrant ) ) {
      snap->test( this, SdPoint( mCenter.x() + mRadius, mCenter.y() ), snapQuadrant );
      snap->test( this, SdPoint( mCenter.x() - mRadius, mCenter.y() ), snapQuadrant );
      snap->test( this, SdPoint( mCenter.x(), mCenter.y() + mRadius ), snapQuadrant );
      snap->test( this, SdPoint( mCenter.x(), mCenter.y() - mRadius ), snapQuadrant );
      }
    }
  }



int SdGraphLinearCircle::behindCursor(SdPoint p)
  {
  if( isVisible() ) {
    if( p.isOnCircle( mCenter, mRadius ) ) {
      return getSelector() ? ELEM_SEL : ELEM_UNSEL;
      }
    }
  return 0;
  }




void SdGraphLinearCircle::saveState(SdUndo *undo)
  {
  undo->propLinePointInt( &mProp, &mCenter, &mRadius );
  }


