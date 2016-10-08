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


/*
  int i = (b.x()-a.x())*(y()-a.y()) - (b.y()-a.y())*(x()-a.x());
  if( i ) return false;
  if( qMin(a.x(),b.x()) <= x() && qMax(a.x(),b.x()) >= x() &&
      qMin(a.y(),b.y()) <= y() && qMax(a.y(),b.y()) >= y() ) return true;
  return false;
*/

bool SdSegment::isCross(SdPoint a, SdPoint b, SdPoint *out ) const
  {
  QPoint dir1 = p2 - p1;
  QPoint dir2 = b - a;

  //считаем уравнения прямых проходящих через отрезки
  double a1 = -dir1.y();
  double b1 = +dir1.x();
  double d1 = -(a1*p1.x() + b1*p1.y());

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
