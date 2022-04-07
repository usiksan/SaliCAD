/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Graphical object filled rectangle.
*/
#include "SdGraphLinearRectFilled.h"
#include "SdSelector.h"
#include "SdSnapInfo.h"
#include "SdContext.h"
#include "Sd3dDraw.h"

SdGraphLinearRectFilled::SdGraphLinearRectFilled() :
  SdGraphLinearRect()
  {

  }




SdGraphLinearRectFilled::SdGraphLinearRectFilled(SdPoint p1, SdPoint p2, const SdPropLine &propLine) :
  SdGraphLinearRect( p1, p2, propLine )
  {

  }




void SdGraphLinearRectFilled::selectByPoint(const SdPoint p, SdSelector *selector)
  {
  if( mProp.mLayer.isEdited() ) {
    SdRect rect(a,b);
    SdPoint p1(p);
    if( !getSelector() && (p1.isOnSegment( rect.getTopLeft(), rect.getBottomLeft() ) ||
                           p1.isOnSegment( rect.getBottomLeft(), rect.getBottomRight() ) ||
                           p1.isOnSegment( rect.getBottomRight(), rect.getTopRight() ) ||
                           p1.isOnSegment( rect.getTopRight(), rect.getTopLeft() ) ||
                           rect.contains(p)) ) {
      //Выбор за одну вершину (нужно производить растяжение/сжатие)
      // перемещаем вершины таким образом, чтобы изменяемой была вершина a
      mFly = 1;
      if( rect.getTopLeft() == p ) {
        a = rect.getTopLeft();
        b = rect.getBottomRight();
        }
      else if( rect.getBottomLeft() == p ) {
        a = rect.getBottomLeft();
        b = rect.getTopRight();
        }
      else if( rect.getBottomRight() == p ) {
        a = rect.getBottomRight();
        b = rect.getTopLeft();
        }
      else if( rect.getTopRight() == p ) {
        a = rect.getTopRight();
        b = rect.getBottomLeft();
        }
      else mFly = 0;
      selector->insert( this );
      }
    }
  }




void SdGraphLinearRectFilled::draw(SdContext *dc)
  {
  dc->fillRect( SdRect(a,b), mProp.mLayer.layer() );
  }




//!
//! \brief draw3d Draws object in 3d space
//! \param f      3d draw functions with predefined 3d context
//!
void SdGraphLinearRectFilled::draw3d(QOpenGLFunctions_2_0 *f) const
  {
  Sd3dDraw::rectFilled( f, a, b, -0.1 );
  }




int SdGraphLinearRectFilled::behindCursor(SdPoint p)
  {
  if( isVisible() ) {
    SdRect rect(a,b);
    if( rect.contains(p,false) ) {
      return getSelector() ? ELEM_SEL : ELEM_UNSEL;
      }
    }
  return 0;
  }



//void SdGraphLinearRectFilled::selectByRect(const SdRect &r, SdSelector *selector)
//  {
//  if( mProp.mLayer.isEdited() ) {
//    SdRect sr(a,b);
//    if( !getSelector() && (r.intersects( sr ) || r.isAccross( sr )) ) {
//      mFly = 0;
//      selector->insert( this );
//      }
//    }
//  }
