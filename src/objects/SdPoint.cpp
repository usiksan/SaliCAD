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

#include <QTransform>
#include <math.h>
//#include <boost/geometry/geometry.hpp>

#if 0
void SdPoint::rotate(SdPoint origin, SdPvAngle angle)
  {
  SdPoint p;
  switch( angle.positive() ) {
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
      //QTransform t;
      double dx = x() - origin.x(), dy = y() - origin.y();
      double sinAngle = sin((M_PI / 180.0) * angle.getDegree()), cosAngle = cos((M_PI / 180.0) * angle.getDegree());
      setX( static_cast<int>(origin.x() + dx*cosAngle - dy*sinAngle) );
      setY( static_cast<int>(origin.y() + dx*sinAngle + dy*cosAngle) );
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
  SdPvAngle ang = b.getAngle(a);
  rotate( SdPoint(), ang.complement() );
  //Зеркализация
  setY( -y() );
  //Обратный поворот
  rotate( SdPoint(), ang );
  //Перенос
  move( a );
  }

#endif



void SdPoint::moveOriented( int dx, int dy, SdOrientation orient)
  {
  switch( orient ) {
    case sorNull   :
      //For p1 == p2
    case sorAny :
      setX( x() + dx );
      setY( y() + dy );
      break;
    case sorVertical :
      //For p1.x == p2.x
      setY( y() + dy );
      break;
    case sorHorizontal :
      //For p1.y == p2.y
      setX( x() + dx );
      break;
    case sorSlashForward :
      //For dx == dy
      setX( x() + dx );
      setY( y() + dy );
      break;
    case sorSlashBackward :
      //For dx == -dy
      setX( x() + dx );
      setY( y() - dy );
      break;
    }
  }




SdPvAngle SdPoint::getAngle(SdPoint center) const
  {
  return SdPvAngle( static_cast<int>(getAngleDegree(center) * 1000.0) );
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





//Return distance between two points - this and p
double SdPoint::getDistance(SdPoint p) const
  {
  return sqrt( getSquareDistance(p) );
  }



//Return square distance between two points
double SdPoint::getSquareDistance(SdPoint p) const
  {
  double dx = x() - p.x();
  double dy = y() - p.y();
  return dx*dx + dy*dy;
  }








double SdPoint::getLenght() const
  {
  double fx = x();
  double fy = y();
  return sqrt( fx * fx + fy * fy );
  }



/*
SdPoint SdPoint::convertImplement(SdPoint origin, SdPoint offset, SdPvAngle angle, bool mirror)
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




SdPoint SdPoint::unConvertImplement(SdPoint origin, SdPoint offset, SdPvAngle angle, bool mirror)
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
*/



bool SdPoint::isOnCircle(SdPoint center, int radius, int delta) const
  {
  //Точка на окружности, если расстояние отсюда до центра равно радиусу
  return abs( radius - getDistanceInt(center) ) <= delta;
  }



bool SdPoint::isInsideCircle(SdPoint center, int radius) const
  {
  return radius >= getDistanceInt(center);
  }



bool SdPoint::isOnArc(SdPoint center, SdPoint start, SdPoint stop, int delta) const
  {
  if( isOnCircle( center, center.getDistanceInt(start), delta )  ) {
    //Расстояние отсюда до центра дуги равно радиусу дуги, проверяем дальше
    return angleVector( start, center, stop ) >= angleVector( start, center, *this );
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







void SdPoint::swap(SdPoint &p)
  {
  int tx = p.x();
  int ty = p.y();
  p.set( x(), y() );
  set( tx, ty );
  }



//!
//! \brief transformMoveFrom Constructs a transformation matrix that translates point p to the current position
//! \param p                 Source translation point
//! \return                  Transformation matrix
//!
QTransform SdPoint::transformMoveFrom(SdPoint p) const
  {
  return QTransform::fromTranslate( x() - p.x(), y() - p.y() );
  }




//!
//! \brief transformRotation Constructs a transformation matrix that performs a rotation by the specified angle around
//!                          a given point that serves as the center of rotation
//! \param angle             Rotation angle
//! \return                  Transformation matrix
//!
QTransform SdPoint::transformRotation(SdPvAngle angle) const
  {
  QTransform map( QTransform::fromTranslate( x(), y() ) );
  map.rotate( angle.getDegree() );
  map.translate( -x(), -y() );
  return map;
  }




//!
//! \brief json Overloaded function to write object content into json writer
//! \param js   Json writer
//!
void SdPoint::json(SvJsonWriter &js) const
  {
  js.jsonInt( QStringLiteral("x"), x() );
  js.jsonInt( QStringLiteral("y"), y() );
  }




//!
//! \brief json Overloaded function to read object content from json reader
//! \param js   Json reader
//!
void SdPoint::json(const SvJsonReader &js)
  {
  js.jsonInt( QStringLiteral("x"), rx() );
  js.jsonInt( QStringLiteral("y"), ry() );
  }




//!
//! \brief angleVector Calculates the rotation angle from point a to point b around center point
//! \param a           First point (start)
//! \param center      Center point of rotation
//! \param b           Second point (end)
//! \return            Angle in degrees in range [0, 360) - counterclockwise rotation from a to b
//!                    Returns 0.0 if a or b coincides with center
//!
double SdPoint::angleVector( SdPoint a, SdPoint center, SdPoint b )
  {
  // Check if center coincides with either point
  if( a == center || b == center ) {
    return 0.0;  // Angle is undefined when center coincides with point
    }

  // Create vectors from center to points
  QPoint vecA = a - center;
  QPoint vecB = b - center;

  // Calculate angles using atan2 (safe even with zero vectors, but we already checked)
  double angleARad = std::atan2(vecA.y(), vecA.x());
  double angleBRad = std::atan2(vecB.y(), vecB.x());

  // Calculate difference
  double angleDiffRad = angleBRad - angleARad;

  // Normalize to range [0, 2π)
  if( angleDiffRad < 0 ) {
    angleDiffRad += 2.0 * M_PI;
    }

  // Convert to degrees
  double angleDeg = angleDiffRad * 180.0 / M_PI;

  // Ensure result is in [0, 360) (handle floating point edge cases)
  if( angleDeg >= 360.0 ) {
    angleDeg -= 360.0;
    }

  // Handle very small negative values due to floating point precision
  if( angleDeg < 0 && angleDeg > -1e-10 ) {
    angleDeg = 0.0;
    }

  return angleDeg;
  }





SdPvAngle calcDirection90( SdPoint a, SdPoint b ) {
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




//Calculate intermediate point to path from a to b
//We use diagonal path. When do it, we can
// draw first diagonal part and next orthogonal part or vice versa
//At this function we calculate most len part first
SdPoint get45( SdPoint a, SdPoint b ) {
  int dx = b.x() - a.x();
  int dy = b.y() - a.y();
  int adx = abs(dx);
  int ady = abs(dy);
  int diag = qMin( adx, ady );
  if( diag == 0 )
    //a-b is strong orthogonal line
    return b;
  int ortho = qMax( adx, ady ) - diag;
  if( ortho == 0 )
    //a-b is strong diagonal line
    return b;
  if( ortho > diag ) {
    //First is orthogonal part
    if( adx > diag )
      //By axiz X
      return SdPoint( a.x() + (dx > 0 ? ortho : -ortho), a.y() );
    //By axiz Y
    return SdPoint( a.x(), a.y() + (dy > 0 ? ortho : - ortho) );
    }
  //First is diagonal part
  return SdPoint( a.x() + ( dx > 0 ? diag : -diag), a.y() + ( dy > 0 ? diag : -diag) );
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
  QTransform map( QTransform::fromTranslate(center.x(),center.y()) );
  map.rotate( SdPoint::angleVector( start, center, sector )  );
  map.translate(-center.x(),-center.y());
  return map.map(start);
  }



//Get intermediate point with 45 degree step vertex
//    result  --------+ b
//           /
//          /
//       a +
//Where orient is b-result orientation
SdPoint get45oriented(SdPoint a, SdPoint b, SdOrientation first, SdOrientation second)
  {
  Q_UNUSED(first);
  Q_UNUSED(second);
  int dx = b.x() - a.x();
  int dy = b.y() - a.y();
  int adx = abs(dx);
  int ady = abs(dy);
  int diag = qMin( adx, ady );
  if( diag == 0 )
    //a-b is strong orthogonal line
    return b;
  int ortho = qMax( adx, ady ) - diag;
  if( ortho == 0 )
    //a-b is strong diagonal line
    return b;
  if( ortho > diag ) {
    //First is orthogonal part
    if( adx > diag )
      //By axiz X
      return SdPoint( a.x() + (dx > 0 ? ortho : -ortho), a.y() );
    //By axiz Y
    return SdPoint( a.x(), a.y() + (dy > 0 ? ortho : - ortho) );
    }
  //First is diagonal part
  return SdPoint( a.x() + ( dx > 0 ? diag : -diag), a.y() + ( dy > 0 ? diag : -diag) );
  }





//Check if three points are all on one line
bool is3PointsOnLine(SdPoint p1, SdPoint p2, SdPoint p3)
  {
  return (p1.x() - p3.x()) * (p2.y() - p3.y()) - ((p2.x() - p3.x()) * (p1.y() - p3.y())) == 0;
  }
