/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Primitive Circle defined by center point and radius
*/

#include "SdCircle.h"
#include "SdRect.h"
#include <math.h>



bool SdCircle::isPointInside(SdPoint p) const
  {
  return p.getDistance(mCenter) <= mRadius;
  }



bool SdCircle::isRectInside(SdRect r) const
  {
  double w = r.width();
  double h = r.height();
  double rr = sqrt( w*w + h*h ) / 2;
  return isCircleInside( SdCircle(r.center(), static_cast<int>(rr) ) );
  }




bool SdCircle::isCircleInside(SdCircle c) const
  {
  double dist = c.mCenter.getDistance(mCenter);
  return dist + c.mRadius <= mRadius;
  }




SdRect SdCircle::overRect() const
  {
  return SdRect( mCenter.x() - mRadius, mCenter.y() - mRadius, mRadius * 2, mRadius * 2 );
  }




SdRect SdCircle::innerRect() const
  {
  double r = mRadius;
  r *= r;
  int w = static_cast<int>(sqrt(r/2));
  return SdRect( mCenter.x() - w, mCenter.y() - w, w * 2, w * 2 );
  }
