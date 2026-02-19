/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/

#include "SdSegment.h"
#include "SdRect.h"

#include <QLineF>
#include <math.h>
#include <QTransform>

/*
  int i = (b.x()-a.x())*(y()-a.y()) - (b.y()-a.y())*(x()-a.x());
  if( i ) return false;
  if( qMin(a.x(),b.x()) <= x() && qMax(a.x(),b.x()) >= x() &&
      qMin(a.y(),b.y()) <= y() && qMax(a.y(),b.y()) >= y() ) return true;
  return false;
*/

void SdSegment::transform(const QTransform &map)
  {
  p1 = map.map(p1);
  p2 = map.map(p2);
  }

void SdSegment::transformP1(const QTransform &map)
  {
  p1 = map.map(p1);
  }

void SdSegment::transformP2(const QTransform &map)
  {
  p2 = map.map(p2);
  }



//Vertex point with 45 degree step
SdPoint SdSegment::vertex45() const
  {
  int dx = p2.x() - p1.x();
  int dy = p2.y() - p1.y();
  if( dx == 0 || dy == 0 || abs(dx) == abs(dy) ) return p2;
  if( abs(dx) < abs(dy) ) {
    //45 degree segment equals dx
    if( dy > 0 ) return SdPoint( p2.x(), p1.y() + abs(dx) );
    return SdPoint( p2.x(), p1.y() - abs(dx) );
    }
  //45 degree segment equals dy
  if( dx > 0 ) return SdPoint( p1.x() + abs(dy), p2.y() );
  return SdPoint( p1.x() - abs(dy), p2.y() );
  }




bool SdSegment::isCross(SdPoint a, SdPoint b, SdPoint *out ) const
  {
  QLineF f1( p1.toPointF(), p2.toPointF() );
  QLineF f2( a.toPointF(), b.toPointF() );
  QPointF of;
  bool res = f1.intersects( f2, &of ) == QLineF::BoundedIntersection;
  if( out )
    *out = of.toPoint();
  return res;
  }




bool SdSegment::isContinue(SdSegment s) const
  {
  if( isSectionX() && s.isSectionX() ) {
    return qMin(p1.x(),p2.x()) <= qMax(s.p1.x(),s.p2.x()) &&
           qMax(p1.x(),p2.x()) >= qMin(s.p1.x(),s.p2.x());
    }
  if( isSectionY() && s.isSectionY() ) {
    return qMin(p1.y(),p2.y()) <= qMax(s.p1.y(),s.p2.y()) &&
           qMax(p1.y(),p2.y()) >= qMin(s.p1.y(),s.p2.y());
    }
  return false;
  }



bool SdSegment::isPointOn(SdPoint p) const
  {
  if( vectorMultiplication(p1,p) != 0 )
    return false;
  return (qMin(p1.x(),p2.x()) <= p.x()) &&
         (qMax(p1.x(),p2.x()) >= p.x()) &&
         (qMin(p1.y(),p2.y()) <= p.y()) &&
         (qMax(p1.y(),p2.y()) >= p.y());
  }



//Векторное произведение
int SdSegment::vectorMultiplication(SdPoint a, SdPoint b) const
  {
  return (b.x()-a.x()) * (p2.y()-p1.y()) - (b.y()-a.y()) * (p2.x()-p1.x());
  }



void SdSegment::calcLineK(double &A, double &B, double &C) const
  {
  //Отыскать уравнение прямой
  int dx = p2.x() - p1.x();
  int dy = p2.y() - p1.y();
  //Коэффициенты уравнения
  A = -dy;
  B = dx;
  C = dy * p1.x() - dx * p2.y();
  }



double SdSegment::lineDistance(SdPoint p) const
  {
  //Частный случай совпадения концов отрезка
  if( p1 == p2 ) return p1.getDistance( p );
  //Коэффициенты уравнения
  double A, B, C;
  calcLineK( A, B, C );
  //Вычислить величину
  //Q_ASSERT( A != 0 || B != 0 );
  double res = ( A * p.x() + B * p.y() + C ) / sqrt( A * A + B * B );
  return res >= 0 ? res : -res;
  }




SdPoint SdSegment::getLineNearest(SdPoint sour) const
  {
  //Частный случай совпадения концов отрезка
  if( p1 == p2 ) return p1;
  //Коэффициенты уравнения
  double A, B, C;
  calcLineK( A, B, C );
  //Q_ASSERT( A != 0 || B != 0 );
  SdPoint p;
  p.rx() = static_cast<int>(-A / (A*A + B*B) * (A*sour.x() + B*sour.y() + C) + sour.x());
  p.ry() = static_cast<int>(-B / (A*A + B*B) * (A*sour.x() + B*sour.y() + C) + sour.y());
  return p;
  }




bool SdSegment::isOneSideLine(const SdSegment &s) const
  {
  //Коэффициенты уравнения
  double A, B, C;
  calcLineK( A, B, C );
  //Вычислить числа условия
  double m1 = A * s.p1.x() + B * s.p1.y() + C;
  double m2 = A * s.p2.x() + B * s.p2.y() + C;
  //Проверить условие
  return (m1 < 0 && m2 < 0) || (m1 > 0 && m2 > 0);
  }




//Calculation segment orientation
SdOrientation SdSegment::orientation() const
  {
  if( p1 == p2 ) return sorNull;
  if( isSectionX() ) return sorHorizontal;
  if( isSectionY() ) return sorVertical;
  int dx = p1.x() - p2.x();
  int dy = p1.y() - p2.y();
  if( dx == dy ) return sorSlashForward;
  if( dx == -dy ) return sorSlashBackward;
  return sorAny;
  }




//Return over rect
SdRect SdSegment::getOverRect() const
  {
  return SdRect(p1,p2);
  }




//!
//! \brief json Overloaded function to write object content into json writer
//! \param js   Json writer
//!
void SdSegment::json(SvJsonWriter &js) const
  {
  js.jsonInt( QStringLiteral("p1.x"), p1.x() );
  js.jsonInt( QStringLiteral("p1.y"), p1.y() );
  js.jsonInt( QStringLiteral("p2.x"), p2.x() );
  js.jsonInt( QStringLiteral("p2.y"), p2.y() );
  }



//!
//! \brief json Overloaded function to read object content from json reader
//! \param js   Json reader
//!
void SdSegment::json(const SvJsonReader &js)
  {
  js.jsonInt( QStringLiteral("p1.x"), p1.rx() );
  js.jsonInt( QStringLiteral("p1.y"), p1.ry() );
  js.jsonInt( QStringLiteral("p2.x"), p2.rx() );
  js.jsonInt( QStringLiteral("p2.y"), p2.ry() );
  }

