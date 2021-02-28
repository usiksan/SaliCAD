/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Graphical object Filled Circle.
*/
#include "SdGraphLinearCircleFilled.h"
#include "SdContext.h"
#include "SdSelector.h"
#include "SdRect.h"
#include "SdSnapInfo.h"
#include "Sd3dDraw.h"

SdGraphLinearCircleFilled::SdGraphLinearCircleFilled() :
  SdGraphLinearCircle()
  {

  }

SdGraphLinearCircleFilled::SdGraphLinearCircleFilled(SdPoint center, int radius, const SdPropLine &propLine) :
  SdGraphLinearCircle( center, radius, propLine )
  {

  }



void SdGraphLinearCircleFilled::selectByPoint(const SdPoint p, SdSelector *selector)
  {
  if( mProp.mLayer.isEdited() ) {
    if( !getSelector() && p.isInsideCircle( mCenter, mRadius ) )
      selector->insert( this );
    }
  }




void SdGraphLinearCircleFilled::draw(SdContext *dc)
  {
  dc->circleFill( mCenter, mRadius, mProp.mLayer.layer() );
  }






//!
//! \brief draw3d Draws object in 3d space
//! \param f      3d draw functions with predefined 3d context
//!
void SdGraphLinearCircleFilled::draw3d(QOpenGLFunctions_2_0 *f) const
  {
  Sd3dDraw::circleFill( f, mCenter, mRadius, -0.1 );
  }




int SdGraphLinearCircleFilled::behindCursor(SdPoint p)
  {
  if( isVisible() ) {
    if( p.isInsideCircle( mCenter, mRadius ) ) {
      return getSelector() ? SEL_ELEM : UNSEL_ELEM;
      }
    }
  return 0;
  }
