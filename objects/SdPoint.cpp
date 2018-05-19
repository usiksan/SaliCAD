/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/

#include "SdPoint.h"
#include "SdProp.h"
#include "SdSegment.h"
#include <math.h>



void SdPoint::rotate(SdPoint origin, SdPropAngle angle)
  {
  SdPoint p;
  switch( angle ) {
    case da0   :
      p.setX( x() - origin.x() );
      p.setY( y() - origin.y() );
      break;
    case da90  :
      p.setX( -y() + origin.y() );
      p.setY(  x() - origin.x() );
      break;
    case da180 :
      p.setX( -x() + origin.x() );
      p.setY( -y() + origin.y() );
      break;
    case da270 :
      p.setX(  y() - origin.y() );
      p.setY( -x() + origin.x() );
      break;
    default :
      //Move to zero point origin
      double dx = x() - origin.x(), dy = y() - origin.y();
      double sinAngle = sin((M_PI / 180.0) * angle.getDegree()), cosAngle = cos((M_PI / 180.0) * angle.getDegree());
      setX( (int)(origin.x() + dx*cosAngle - dy*sinAngle) );
      setY( (int)(origin.y() + dx*sinAngle + dy*cosAngle) );
      return;
    }
  p.move( origin );
  set( p.x(), p.y() );
  }




void SdPoint::mirror(SdPoint origin)
  {
  setX( origin.x() - ( x() - origin.x() ) );
  }




void SdPoint::mirror(SdPoint a, SdPoint b)
  {
  /*Идея: переносим начало координат в точку а, поворачиваем плоскость до точки b
          таким образом ab образуют ось 0х, производим зеркализацию по y и делаем
          обратные преобразования
  */
  //Переносим
  move( a.complement() );
  //Поворачиваем
  SdPropAngle ang = b.getAngle(a);
  rotate( SdPoint(), ang.complement() );
  //Зеркализация
  setY( -y() );
  //Обратный поворот
  rotate( SdPoint(), ang );
  //Перенос
  move( a );
  }




SdPropAngle SdPoint::getAngle(SdPoint center) const
  {
  return SdPropAngle( (int)(getAngleDegree(center) * 1000.0) );
  }



double SdPoint::getAngleDegree(SdPoint center) const
  {
  double dx = x() - center.x();
  double dy = y() - center.y();
  double radius = getDistance(center);
  double ang;
  if( radius > 0 ) {
    if( fabs(dx) > fabs(dy) )
      ang = acos( fabs(dx) / radius ) * 90.0 / M_PI_2;
    else
      ang = asin( fabs(dy) / radius ) * 90.0 / M_PI_2;
    if( dx < 0 ) ang = 180 - ang;
    if( dy < 0 ) ang = 360 - ang;
    return ang;
    }
  return 0.0;
  }





double SdPoint::getDistance(SdPoint p) const
  {
  return sqrt( getQrtDistance(p) );
  }



double SdPoint::getQrtDistance(SdPoint p) const
  {
  double dx = x() - p.x();
  double dy = y() - p.y();
  return dx*dx + dy*dy;
  }




SdPoint::CLS SdPoint::classify(SdPoint p0, SdPoint p1) const
  {
  QPointF a(p1 - p0);
  QPointF b( *this - p0);
  double sa = a.x() * b.y() - b.x() * a.y();
  if( sa > 0.0 )
    return LEFT;
  if( sa < 0.0 )
    return RIGHT;
  if( isEqual( p0 ) )
    return ORIGIN;
  if( isEqual( p1 ) )
    return DESTINATION;
  if( (a.x() * b.x() < 0.0) || (a.y() * b.y() < 0.0) )
    return BEHIND;
  if( sqrt(a.x()*a.x() + a.y() * a.y()) < sqrt(a.x()*a.x() + a.y() * a.y()) )
    return BEYOND;
  return BETWEEN;
  }




double SdPoint::getLenght() const
  {
  double fx = x();
  double fy = y();
  return sqrt( fx * fx + fy * fy );
  }




SdPoint SdPoint::convertImplement(SdPoint origin, SdPoint offset, SdPropAngle angle, bool mirror)
  {
  //Образовать точку и перенести начало координат в origin
  SdPoint p(x()-origin.x(),y()-origin.y());
  //Если требуется, произвести зеркальность
  if( mirror ) p.setX( -p.x() );
  //Поворот
  p.rotate( SdPoint(), angle );
  p.move( offset );
  return p;
  }




SdPoint SdPoint::unConvertImplement(SdPoint origin, SdPoint offset, SdPropAngle angle, bool mirror)
  {
  //Образовать точку и перенести начало координат в offset
  SdPoint p(x()-offset.x(),y()-offset.y());
  //Поворот
  p.rotate( SdPoint(), angle.complement() );
  //Если требуется, произвести зеркальность
  if( mirror ) p.setX( -p.x() );
  //Сместить на origin
  p.move( origin );
  return p;
  }




bool SdPoint::isOnCircle(SdPoint center, int radius, int delta) const
  {
  //Точка на окружности, если расстояние отсюда до центра равно радиусу
  return abs( radius - getDistance(center) ) <= delta;
  }



bool SdPoint::isInsideCircle(SdPoint center, int radius) const
  {
  return radius >= getDistance(center);
  }



bool SdPoint::isOnArc(SdPoint center, SdPoint start, SdPoint stop, int delta) const
  {
  if( isOnCircle( center, center.getDistance(start), delta )  ) {
    //Расстояние отсюда до центра дуги равно радиусу дуги, проверяем дальше
    SdPropAngle anStart = start.getAngle(center);
    SdPropAngle anStop  = stop.getAngle(center);
    SdPropAngle cur     = getAngle(center);
    return cur - anStart < anStop - anStart;
    }
  return false;
  }




bool SdPoint::isLeftHigh(SdPoint a) const
  {
  return x() < a.x() || (x() == a.x() && y() > a.y());
  }



bool SdPoint::isOnSegment(SdPoint a, SdPoint b) const
  {
  return SdSegment(a,b).isPointOn( *this );
  /*
  SdPoint pa( b - a );
  SdPoint pb( SdPoint(x(),y()) - a );

  const int cr = pa.x()*pb.y() - pb.x()*pa.y();
  if((cr > 0) || (cr < 0) || (pa.x()*pb.x() < 0) ||
    (pa.y()*pb.y() < 0) || (pa.x()*pa.x()+pa.y()*pa.y() < pb.x()*pb.x()+pb.y()*pb.y()))
    return false;
  return true;
*/
  //If segment is vertical
/*  if( a.x() == b.x() )
    return ( x() == a.x() && y() >= qMin(a.y(), b.y()) && y() <= qMax( a.y(), b.y()) );

  if( a.y() == b.y() )
    return ( y() == a.y() && x() >= qMin(a.x(), b.x()) && x() <= qMax( a.x(), b.x()) );

  double k, c;
  QPointF af(a);
  QPointF bf(b);
  k = (af.y() - bf.y()) / (af.x() - bf.x());
  c = af.y() - k * af.x();

  return y() == (static_cast<double>(x()) * k + c);
  */
  }





void SdPoint::write(const QString name, QJsonObject &obj) const
  {
  obj.insert( name, write() );
  }




void SdPoint::read(const QString name, const QJsonObject obj)
  {
  read( obj.value(name).toObject() );
  }




void SdPoint::swap(SdPoint *p)
  {
  int tx = p->x();
  int ty = p->y();
  p->set( x(), y() );
  set( tx, ty );
  }




QJsonObject SdPoint::write() const
  {
  QJsonObject obj;
  obj.insert( QString("x"), QJsonValue(x()) );
  obj.insert( QString("y"), QJsonValue(y()) );
  return obj;
  }




void SdPoint::read(const QJsonObject obj)
  {
  setX( obj.value( QString("x") ).toInt() );
  setY( obj.value( QString("y") ).toInt() );
  }



SdPropAngle calcDirection90( SdPoint a, SdPoint b ) {
  if( abs(b.x() - a.x()) > abs(b.y() - a.y()) )
    //Приоритетная ось X
    return b.x() > a.x() ? da0 : da180;
  else
    //Приоритетная ось Y
    return b.y() > a.y() ? da90 : da270;
  }



SdPoint get90( SdPoint a, SdPoint b ) {
  if( abs(b.x() - a.x()) > abs(b.y() - a.y()) ) return SdPoint(b.x(),a.y());
  return SdPoint(a.x(),b.y());
  }



SdPoint get45( SdPoint a, SdPoint b ) {
  int dx = b.x() - a.x();
  int dy = b.y() - a.y();
  int len = ( abs( dx ) > abs( dy ) ) ? abs(dy) : abs(dx);
  if( !len ) return b;
  return SdPoint( a.x() + ( dx > 0 ? len : -len), a.y() + ( dy > 0 ? len : -len) );
  }




SdPoint calcMiddlePoint(SdPoint a, SdPoint b, int enterType )
  {
  switch( enterType ) {
    case dleOrtho     : return get90(a,b);
    case dle45degree  : return get45(a,b);
    default           : return b;
    }
  }




int sred(int a, int b, int gridSize)
  {
  if( gridSize < 1 ) gridSize = 1;
  return (((a + b) / 2) / gridSize) * gridSize;
  }




bool calcFreeNearIntersect(SdPoint sour, SdPoint a, SdPoint b, SdPoint &dest)
  {
  if( a.x() == b.x() ) {
    //Вертикальная линия
    dest.rx() = a.x();
    if( sour.y() >= qMin(a.y(),b.y()) && sour.y() <= qMax(a.y(),b.y()) ) {
      //Ближайшая точка находится на отрезке
      dest.ry() = sour.y();
      }
    else {
      //Ближайшая точка это один из концов отрезка
      if( sour.y() < qMin(a.y(),b.y()) ) dest.ry() = qMin(a.y(),b.y());
      else dest.ry() = qMax(a.y(),b.y());
      }
    }
  else if( a.y() == b.y() ) {
    //Горизонтальная линия
    dest.ry() = a.y();
    if( sour.x() >= qMin(a.x(),b.x()) && sour.x() <= qMax(a.x(),b.x()) ) {
      //Ближайшая точка находится на отрезке
      dest.rx() = sour.x();
      }
    else {
      //Ближайшая точка это один из концов отрезка
      if( sour.x() < qMin(a.x(),b.x()) ) dest.rx() = qMin(a.x(),b.x());
      else dest.rx() = qMax(a.x(),b.x());
      }
    }
  else return false;
  return true;
  }




SdPoint calcArcStop(SdPoint center, SdPoint start, SdPoint sector)
  {
  double radius = center.getDistance(start);
  SdPoint stop((int)(center.x()+radius),center.y());
  stop.rotate( center, sector.getAngle(center) );
  return stop;
  }
