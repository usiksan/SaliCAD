/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Graphical object rectangle.
*/
#include "SdGraphLinearRect.h"
#include "SdSelector.h"
#include "SdSnapInfo.h"
#include "SdContext.h"
#include "Sd3dDraw.h"
#include "SdJsonIO.h"

SdGraphLinearRect::SdGraphLinearRect() :
  SdGraphLinear(),
  mFly(0)
  {

  }

SdGraphLinearRect::SdGraphLinearRect(SdPoint p1, SdPoint p2, const SdPropLine &propLine) :
  SdGraphLinear(propLine),
  a(p1),
  b(p2),
  mFly(0)
  {
  }



SdPointList SdGraphLinearRect::getPointList() const
  {
  SdPointList list;
  int left = qMin( a.x(), b.x() );
  int right = qMax( a.x(), b.x() );
  int top = qMax( a.y(), b.y() );
  int bot = qMin( a.y(), b.y() );
  list.append( SdPoint(left,bot) );
  list.append( SdPoint(left,top) );
  list.append( SdPoint(right,top) );
  list.append( SdPoint(right,bot) );
  return list;
  }




QString SdGraphLinearRect::getType() const
  {
  return QStringLiteral(SD_TYPE_RECT);
  }




quint64 SdGraphLinearRect::getClass() const
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
void SdGraphLinearRect::cloneFrom(const SdObject *src, SdCopyMap &copyMap, bool next)
  {
  SdGraphLinear::cloneFrom( src, copyMap, next );
  SdPtrConst<SdGraphLinearRect> rect(src);
  if( rect.isValid() ) {
    a = rect->a;
    b = rect->b;
    }
  }




//!
//! \brief json Overloaded function to write object content into json writer
//!             Overrided function
//! \param js   Json writer
//!
void SdGraphLinearRect::json(SdJsonWriter &js) const
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
void SdGraphLinearRect::json(const SdJsonReader &js)
  {
  js.jsonPoint( QStringLiteral("a"), a );
  js.jsonPoint( QStringLiteral("b"), b );
  SdGraphLinear::json( js );
  }










void SdGraphLinearRect::move(SdPoint offset)
  {
  a.move( offset );
  if( !mFly ) b.move( offset );
  }




void SdGraphLinearRect::rotate(SdPoint center, SdPropAngle angle)
  {
  a.rotate( center, angle );
  b.rotate( center, angle );
  }




void SdGraphLinearRect::mirror(SdPoint p1, SdPoint p2)
  {
  a.mirror( p1, p2 );
  b.mirror( p1, p2 );
  }




void SdGraphLinearRect::selectByPoint(const SdPoint p, SdSelector *selector)
  {
  if( mProp.mLayer.isEdited() ) {
    SdRect rect(a,b);
    SdPoint p1(p);
    if( !getSelector() && (p1.isOnSegment( rect.getTopLeft(), rect.getBottomLeft() ) ||
                           p1.isOnSegment( rect.getBottomLeft(), rect.getBottomRight() ) ||
                           p1.isOnSegment( rect.getBottomRight(), rect.getTopRight() ) ||
                           p1.isOnSegment( rect.getTopRight(), rect.getTopLeft() ) ) ) {
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




void SdGraphLinearRect::selectByRect(const SdRect &r, SdSelector *selector)
  {
  if( mProp.mLayer.isEdited() ) {
    if( !getSelector() && r.isAccross( SdRect(a,b) ) ) {
      mFly = 0;
      selector->insert( this );
      }
    }
  }




void SdGraphLinearRect::select(SdSelector *selector)
  {
  mFly = 0;
  if( selector != nullptr )
    selector->insert( this );
  }




SdRect SdGraphLinearRect::getOverRect() const
  {
  return SdRect(a,b);
  }




void SdGraphLinearRect::draw(SdContext *dc)
  {
  dc->rect( SdRect(a,b), mProp );
  }




//!
//! \brief draw3d Draws object in 3d space
//! \param f      3d draw functions with predefined 3d context
//!
void SdGraphLinearRect::draw3d(QOpenGLFunctions_2_0 *f) const
  {
  Sd3dDraw::rect( f, a, b, -0.1 );
  }




void SdGraphLinearRect::accumHoles(Sd3drModel &model, Sd3drFaceList &faceList, SdStratum stratum, const QMatrix4x4 &map) const
  {
  Q_UNUSED(stratum)
  QMatrix4x4 mat(map);
  SdRect r(a,b);
  QVector3D v( r.center() );
  v /= 1000.0;
  mat.translate( v );
  faceList.append( model.faceRectangle( static_cast<float>(r.width()) / 1000.0, static_cast<float>(r.height()) / 1000.0, mat ) );
  }




int SdGraphLinearRect::behindCursor(SdPoint p)
  {
  if( isVisible() ) {
    SdRect rect(a,b);
    if( p.isOnSegment( rect.getTopLeft(), rect.getBottomLeft() ) ||
        p.isOnSegment( rect.getBottomLeft(), rect.getBottomRight() ) ||
        p.isOnSegment( rect.getBottomRight(), rect.getTopRight() ) ||
        p.isOnSegment( rect.getTopRight(), rect.getTopLeft() ) ) {
          return getSelector() ? ELEM_SEL : ELEM_UNSEL;
          }
    }
  return 0;
  }




//Find snap point on object
void SdGraphLinearRect::snapPoint(SdSnapInfo *snap)
  {
  if( isVisible() ) {
    SdRect r(a,b);
    if( snap->match(snapEndPoint) ) {
      snap->test( this, r.getTopLeft(), snapEndPoint );
      snap->test( this, r.getBottomLeft(), snapEndPoint );
      snap->test( this, r.getTopRight(), snapEndPoint );
      snap->test( this, r.getBottomRight(), snapEndPoint );
      }
    if( snap->match(snapMidPoint) ) {
      snap->test( this, SdPoint(r.getTopLeft()).getMiddle( r.getTopRight() ), snapMidPoint );
      snap->test( this, SdPoint(r.getTopRight()).getMiddle( r.getBottomRight() ), snapMidPoint );
      snap->test( this, SdPoint(r.getBottomRight()).getMiddle( r.getBottomLeft() ), snapMidPoint );
      snap->test( this, SdPoint(r.getBottomLeft()).getMiddle( r.getTopLeft() ), snapMidPoint );
      }
    }
  }




void SdGraphLinearRect::saveState(SdUndo *undo)
  {
  undo->propLineAnd3Point( &mProp, &a, &b );
  }
