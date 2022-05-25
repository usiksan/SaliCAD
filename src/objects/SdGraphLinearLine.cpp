/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Graphical object Line. Line is linear segment with two ends.
*/
#include "SdGraphLinearLine.h"
#include "SdSegment.h"
#include "SdContext.h"
#include "SdSelector.h"
#include "Sd3dDraw.h"
#include "SdJsonIO.h"

SdGraphLinearLine::SdGraphLinearLine() :
  SdGraphLinear(),
  a(),
  b(),
  mFlyA(false),
  mFlyB(false)
  {

  }

SdGraphLinearLine::SdGraphLinearLine(SdPoint p1, SdPoint p2, const SdPropLine &propLine) :
  SdGraphLinear( propLine ),
  a(p1),
  b(p2),
  mFlyA(false),
  mFlyB(false)
  {

  }




QString SdGraphLinearLine::getType() const
  {
  return QStringLiteral(SD_TYPE_LINE);
  }



quint64 SdGraphLinearLine::getClass() const
  {
  return dctLines;
  }




//!
//! \brief cloneFrom Overrided function. We copy object from source
//! \param src       Source of object from which copy must be made
//! \param copyMap   Structure for mapping copying substitutes
//! \param next      Make simple or next copy. Next copy available not for all objects.
//!                  For example: pin name A23 with next copy return A24
//!
void SdGraphLinearLine::cloneFrom(const SdObject *src, SdCopyMap &copyMap, bool next)
  {
  SdGraphLinear::cloneFrom( src, copyMap, next );
  SdPtrConst<SdGraphLinearLine> line(src);
  if( line.isValid() ) {
    a = line->a;
    b = line->b;
    }
  }



//!
//! \brief json Overloaded function to write object content into json writer
//!             Overrided function
//! \param js   Json writer
//!
void SdGraphLinearLine::json(SdJsonWriter &js) const
  {
  js.jsonPoint( QStringLiteral("a"), a );
  js.jsonPoint( QStringLiteral("b"), b );
  SdGraphLinear::json( js );
  }





//!
//! \brief json Overloaded function to read object content from json reader
//!             Overrided function
//! \param js   Json reader
//!
void SdGraphLinearLine::json(const SdJsonReader &js)
  {
  js.jsonPoint( QStringLiteral("a"), a );
  js.jsonPoint( QStringLiteral("b"), b );
  SdGraphLinear::json( js );
  }








void SdGraphLinearLine::move(SdPoint offset)
  {
  //Перенести
  if( mFlyA ) a.move( offset );
  if( mFlyB ) b.move( offset );
  }




void SdGraphLinearLine::rotate(SdPoint center, SdPropAngle angle)
  {
  //Повернуть
  if( mFlyA ) a.rotate( center, angle );
  if( mFlyB ) b.rotate( center, angle );
  }




void SdGraphLinearLine::mirror(SdPoint a, SdPoint b)
  {
  //Отразить (зеркальность) относительно линии
  a.mirror( a, b );
  b.mirror( a, b );
  }




void SdGraphLinearLine::selectByPoint(const SdPoint p, SdSelector *selector)
  {
  if( mProp.mLayer.isEdited() ) {
    if( !getSelector() && SdSegment(a,b).isPointOn( p ) ) {
      if( a == p ) { mFlyA = true; mFlyB = false; }
      else if( b == p ) { mFlyA = false; mFlyB = true; }
      else mFlyA = mFlyB = true;
      selector->insert( this );
      }
    }
  }



void SdGraphLinearLine::selectByRect(const SdRect &r, SdSelector *selector)
  {
  if( mProp.mLayer.isEdited() ) {
    if( !getSelector() && r.isAccross(a,b) ) {
      mFlyA = mFlyB = true;
      selector->insert( this );
      }
    }
  }




void SdGraphLinearLine::select(SdSelector *selector)
  {
  mFlyA = mFlyB = true;
  if( selector != nullptr )
    selector->insert( this );
  }




SdRect SdGraphLinearLine::getOverRect() const
  {
  return SdRect( a, b );
  }




void SdGraphLinearLine::draw(SdContext *dc)
  {
  dc->line( a, b, mProp );
  }




//!
//! \brief draw3d Draws object in 3d space
//! \param f      3d draw functions with predefined 3d context
//!
void SdGraphLinearLine::draw3d(QOpenGLFunctions_2_0 *f) const
  {
  Sd3dDraw::line( f, a, b, -0.1 );
  }



//Find snap point on object
void SdGraphLinearLine::snapPoint(SdSnapInfo *snap)
  {
  if( isVisible() ) {
    if( snap->match(snapEndPoint) ) {
      snap->test( this, a, snapEndPoint );
      snap->test( this, b, snapEndPoint );
      }
    if( snap->match(snapMidPoint) )
      snap->test( this, a.getMiddle( b ), snapMidPoint );
    }
  }



int SdGraphLinearLine::behindCursor(SdPoint p)
  {
  //Определить состояние объекта под курсором
  if( isVisible() && SdSegment(a,b).isPointOn( p ) )
    return getSelector() ? ELEM_SEL : ELEM_UNSEL;
  return 0;
  }





void SdGraphLinearLine::saveState(SdUndo *undo)
  {
  undo->propLineAnd3Point( &mProp, &a, &b );
  }
