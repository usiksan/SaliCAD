/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/

#include "SdCircle.h"
#include "SdRect.h"
#include <math.h>



bool SdCircle::isPointInside(SdPoint p) const
  {
  return p.getDistance(center) <= radius;
  }



bool SdCircle::isRectInside(SdRect r) const
  {
  double w = r.width();
  double h = r.height();
  double rr = sqrt( w*w + h*h ) / 2;
  return isCircleInside( SdCircle(r.center(), rr) );
  }




bool SdCircle::isCircleInside(SdCircle c) const
  {
  double dist = c.center.getDistance(center);
  return dist + c.radius <= radius;
  }

SdRect SdCircle::overRect() const
  {
  return SdRect( center.x() - radius, center.y() - radius, radius * 2, radius * 2 );
  }

SdRect SdCircle::innerRect() const
  {
  double r = radius;
  r *= r;
  int w = sqrt(r/2);
  return SdRect( center.x() - w, center.y() - w, w * 2, w * 2 );
  }
