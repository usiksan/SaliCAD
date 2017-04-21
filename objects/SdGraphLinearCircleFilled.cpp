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
  if( isAble() ) {
    if( !getSelector() && p.isInsideCircle( mCenter, mRadius ) )
      selector->insert( this );
    }
  }




void SdGraphLinearCircleFilled::draw(SdContext *dc)
  {
  dc->circleFill( mCenter, mRadius, mProp );
  }




int SdGraphLinearCircleFilled::behindCursor(SdPoint p)
  {
  if( isAble() ) {
    if( p.isInsideCircle( mCenter, mRadius ) ) {
      return getSelector() ? SEL_ELEM : UNSEL_ELEM;
      }
    }
  return 0;
  }
