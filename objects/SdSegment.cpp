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
#include <math.h>

/*
  int i = (b.x()-a.x())*(y()-a.y()) - (b.y()-a.y())*(x()-a.x());
  if( i ) return false;
  if( qMin(a.x(),b.x()) <= x() && qMax(a.x(),b.x()) >= x() &&
      qMin(a.y(),b.y()) <= y() && qMax(a.y(),b.y()) >= y() ) return true;
  return false;
*/

bool SdSegment::isCross(SdPoint a, SdPoint b, SdPoint *out ) const
  {
  Q_UNUSED(out)
  SdSegment s(a,b);
  return !isOneSideLine( s ) && !s.isOneSideLine( *this );
#if 0
  QPoint dir1 = p2 - p1;
  QPoint dir2 = b - a;

  //считаем уравнения прямых проходящих через отрезки
  double a1 = -dir1.y();
  double b1 = +dir1.x();
  double d1 = -(a1*p1.x() + b1*p2.y());

  double a2 = -dir2.y();
  double b2 = +dir2.x();
  double d2 = -(a2*a.x() + b2*b.y());

  //подставляем концы отрезков, для выяснения в каких полуплоскотях они
  double seg1_line2_start = a2*p1.x() + b2*p1.y() + d2;
  double seg1_line2_end = a2*p2.x() + b2*p2.y() + d2;

  double seg2_line1_start = a1*a.x() + b1*a.y() + d1;
  double seg2_line1_end = a1*b.x() + b1*b.y() + d1;

  //если концы одного отрезка имеют один знак, значит он в одной полуплоскости и пересечения нет.
  if( seg1_line2_start * seg1_line2_end >= 0 || seg2_line1_start * seg2_line1_end >= 0 )
    return false;

  if( out ) {
    double u = seg1_line2_start / (seg1_line2_start - seg1_line2_end);
    *out = p1 + u*dir1;
    }

  return true;
#endif
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



void SdSegment::calcLineK(int &A, int &B, int &C) const
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
  int A, B, C;
  calcLineK( A, B, C );
  //Вычислить величину
  Q_ASSERT( A != 0 || B != 0 );
  double res = ( A * p.x() + B * p.y() + C ) / sqrt( A * A + B * B );
  return res >= 0 ? res : -res;
  }




SdPoint SdSegment::getLineNearest(SdPoint sour) const
  {
  //Частный случай совпадения концов отрезка
  if( p1 == p2 ) return p1;
  //Коэффициенты уравнения
  int A, B, C;
  calcLineK( A, B, C );
  Q_ASSERT( A != 0 || B != 0 );
  SdPoint p;
  p.rx() = -A / (A*A + B*B) * (A*sour.x() + B*sour.y() + C) + sour.x();
  p.ry() = -B / (A*A + B*B) * (A*sour.x() + B*sour.y() + C) + sour.y();
  return p;
  }




bool SdSegment::isOneSideLine(const SdSegment &s) const
  {
  //Коэффициенты уравнения
  int A, B, C;
  calcLineK( A, B, C );
  //Вычислить числа условия
  int m1 = A * s.p1.x() + B * s.p1.y() + C;
  int m2 = A * s.p2.x() + B * s.p2.y() + C;
  //Проверить условие
  return (m1 < 0 && m2 < 0) || (m1 > 0 && m2 > 0);
  }


