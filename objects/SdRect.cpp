/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
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
  setWidth( qMax(a.x(),b.x()) - left() );
  setHeight( qMax(a.y(),b.y()) - top() );
  }




void SdRect::set(QRect r)
  {
//  set( r.topLeft(), bottomRight() );
  setLeft( r.left() );
  setTop( r.top() );
  setWidth( r.width() );
  setHeight( r.height() );
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
  else
    set( united( rect ) );
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




bool SdRect::isCircleInside(SdCircle circle) const
  {
  return contains( circle.overRect() );
  }




bool SdRect::isAccross(const SdPoint p1, const SdPoint p2) const
  {
  if( isPointInside(p1) || isPointInside(p2) ) return true;
  SdSegment s(p1,p2);
  return s.isCross( getTopLeft(), getTopRight() ) ||
      s.isCross( getTopRight(), getBottomRight() ) ||
      s.isCross( getBottomRight(), getBottomLeft() ) ||
      s.isCross( getBottomLeft(), getTopLeft() );
  }




bool SdRect::isAccross(const SdRect &rect) const {
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
  QJsonObject obj;
  obj.insert( QStringLiteral("x"), left() );
  obj.insert( QStringLiteral("y"), top() );
  obj.insert( QStringLiteral("w"), width() );
  obj.insert( QStringLiteral("h"), height() );
  return obj;
  }




void SdRect::write(const QString name, QJsonObject &obj) const
  {
  obj.insert( name, write() );
  }




void SdRect::read(const QJsonObject &obj)
  {
  setLeft( obj.value( QStringLiteral("x") ).toInt() );
  setTop( obj.value( QStringLiteral("y") ).toInt() );
  setWidth( obj.value( QStringLiteral("w") ).toInt() );
  setHeight( obj.value( QStringLiteral("h") ).toInt() );
  }




void SdRect::read(const QString name, const QJsonObject obj)
  {
  read( obj.value(name).toObject() );
  }




SdRect SdRect::swap(const SdRect &r)
  {
  SdRect tmp( *this );
  set( r );
  return tmp;
  }

