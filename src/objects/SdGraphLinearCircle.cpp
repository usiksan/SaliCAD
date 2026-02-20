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
#include "SdJsonIO.h"
#include "3dr/Sd3drModel.h"

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






//!
//! \brief cloneFrom Overrided function. We copy object from source
//! \param src       Source of object from which copy must be made
//! \param copyMap   Structure for mapping copying substitutes
//! \param next      Make simple or next copy. Next copy available not for all objects.
//!                  For example: pin name A23 with next copy return A24
//!
void SdGraphLinearCircle::cloneFrom(const SdObject *src, SdCopyMap &copyMap, bool next)
  {
  SdGraphLinear::cloneFrom( src, copyMap, next );
  SdPtrConst<SdGraphLinearCircle> circle(src);
  if( circle.isValid() ) {
    mCenter = circle->mCenter;
    mRadius = circle->mRadius;
    }
  }




//!
//! \brief json Overloaded function to write object content into json writer
//!             Overrided function
//! \param js   Json writer
//!
void SdGraphLinearCircle::json(SdJsonWriter &js) const
  {
  js.jsonPoint( QStringLiteral("center"), mCenter );
  js.jsonInt( QStringLiteral("radius"), mRadius );
  SdGraphLinear::json( js );
  }





//!
//! \brief json Overloaded function to read object content from json reader
//!             Overrided function
//! \param js   Json reader
//!
void SdGraphLinearCircle::json(const SdJsonReader &js)
  {
  js.jsonPoint( QStringLiteral("center"), mCenter );
  js.jsonInt( QStringLiteral("radius"), mRadius );
  SdGraphLinear::json( js );
  }





void SdGraphLinearCircle::transform(const QTransform &map, SdPvAngle)
  {
  mCenter = map.map(mCenter);
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




//!
//! \brief accumHoles Accum holes description into faceList
//! \param model      Model which accumulate coord vertexes
//! \param faceList   Face list for holding holes (single face for single hole)
//! \param stratum    Stratum for layers
//! \param map        Map for holes conversion
//!
void SdGraphLinearCircle::accumHoles(Sd3drModel &model, Sd3drFaceList &faceList, SdPvStratum stratum, const QMatrix4x4 &map) const
  {
  Q_UNUSED(stratum)
  QMatrix4x4 mat(map);
  QVector3D v(mCenter);
  v /= 1000.0;
  mat.translate( v );
  faceList.append( model.faceCircleSide( static_cast<float>(mRadius) / 1000.0, 32, mat ) );
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
  undo->prop( &mProp, &mCenter, &mRadius );
  }


