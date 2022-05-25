/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Rect by left top and right bottom corners

  Because Y axiz point to down, then "top" in fact is bottom and wise wersa

  Because right() return (left pos) + width - 1, then replace all functions on correct (left pos) + width
*/

#include "SdRect.h"
#include "SdSegment.h"
#include "SdCircle.h"


SdRect::SdRect(SdPoint a, SdPoint b)
  {
  set( a, b );
  }




SdRect::SdRect(int px, int py, int w, int h)
  {
  set( SdPoint(px,py), SdPoint(px+w,py+h) );
  }




void SdRect::set(SdPoint a, SdPoint b)
  {
  setLeft( qMin(a.x(),b.x()) );
  setTop( qMin(a.y(),b.y()) );
  setRight( qMax(a.x(),b.x()) );
  setBottom( qMax(a.y(),b.y()) );
  }




void SdRect::set(QRect r)
  {
//  set( r.topLeft(), bottomRight() );
  setLeft( r.left() );
  setTop( r.top() );
  setRight( r.right() );
  setBottom( r.bottom() );
  }




void SdRect::grow(SdPoint point)
  {
  if( isEmpty() ) set(point,point);
  else {
    if( point.x() < left() ) setLeft( point.x() );
    if( point.x() > right() ) setRight( point.x() );
    if( point.y() < top() ) setTop( point.y() );
    if( point.y() > bottom() ) setBottom( point.y() );
    }
  }




void SdRect::grow(const SdRect &rect)
  {
  if( isEmpty() )
    set( rect );
  else {
    grow( rect.getTopLeft() );
    grow( rect.getBottomRight() );
    //set( united( rect ) );
    }
  }




void SdRect::mirror(SdPoint origin)
  {
  SdPoint p1 = getTopLeft();
  SdPoint p2 = getBottomRight();
  p1.mirror(origin);
  p2.mirror(origin);
  set( p1, p2 );
  }



void SdRect::rotate(SdPoint origin, SdPropAngle angle)
  {
  SdPoint p1 = getTopLeft();
  SdPoint p2 = getBottomRight();
  p1.rotate(origin,angle);
  p2.rotate(origin,angle);
  set( p1, p2 );
  }




//Test if point inside rectangle or on its border
bool SdRect::isPointInside(const SdPoint point) const
  {
  return getTop() >= point.y() && getBottom() <= point.y() &&
         getLeft() <= point.x() && getRight() >= point.x();
  }




bool SdRect::isCircleInside(SdCircle circle) const
  {
  return contains( circle.overRect() );
  }



bool SdRect::isAccross(const SdSegment &s) const
  {
  if( isPointInside(s.getP1()) || isPointInside(s.getP2()) ) return true;
  return s.isCross( getTopLeft(), getTopRight() ) ||
      s.isCross( getTopRight(), getBottomRight() ) ||
      s.isCross( getBottomRight(), getBottomLeft() ) ||
      s.isCross( getBottomLeft(), getTopLeft() );
  }




bool SdRect::isAccross(const SdPoint p1, const SdPoint p2) const
  {
  return isAccross( SdSegment(p1,p2) );
  }




bool SdRect::isAccross(const SdRect &rect) const {
//  return intersects(rect);
  return isAccross( rect.getTopLeft(), rect.getBottomLeft() ) ||
         isAccross( rect.getTopLeft(), rect.getTopRight() ) ||
         isAccross( rect.getBottomLeft(), rect.getBottomRight() ) ||
         isAccross( rect.getTopRight(), rect.getBottomRight() );
  }




void SdRect::calcOverPolygon(SdPointList &polygon)
  {
  for( SdPoint p : polygon )
    grow(p);
  }







QJsonObject SdRect::write() const
  {
  SvJsonWriter js;
  json( js );
  return js.object();
  }




void SdRect::write(const QString name, QJsonObject &obj) const
  {
  obj.insert( name, write() );
  }




void SdRect::read(const QJsonObject &obj)
  {
  setLeft( obj.value( QStringLiteral("x") ).toInt() );
  setTop( obj.value( QStringLiteral("y") ).toInt() );
  if( obj.contains(QStringLiteral("w")) ) {
    setWidth( obj.value( QStringLiteral("w") ).toInt() );
    setHeight( obj.value( QStringLiteral("h") ).toInt() );
    }
  else {
    setRight( obj.value( QStringLiteral("r") ).toInt() );
    setBottom( obj.value( QStringLiteral("b") ).toInt() );
    }
  }




void SdRect::read(const QString name, const QJsonObject obj)
  {
  read( obj.value(name).toObject() );
  }




void SdRect::json(SvJsonWriter &js) const
  {
  js.jsonInt( "x", left() );
  js.jsonInt( "y", top() );
  js.jsonInt( "r", right() );
  js.jsonInt( "b", bottom() );
  }



void SdRect::json(SvJsonReader &js)
  {
  read( js.object() );
  }




SdRect SdRect::swap(const SdRect &r)
  {
  SdRect tmp( *this );
  set( r );
  return tmp;
  }

