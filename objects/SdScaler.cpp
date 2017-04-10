/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/
#include "SdConfig.h"
#include "SdScaler.h"

SdScaler::SdScaler(double scale) :
  mScale(scale)
  {

  }

void SdScaler::scale(double step)
  {
  //Convert scale
  mScale *= step;
  //Check scale bounds
  if( mScale > maxScale ) mScale = maxScale;
  if( mScale < minScale ) mScale = minScale;
  }



int SdScaler::phys2pixel(int phys) const
  {
  return ((double)phys) / mScale;
  }

int SdScaler::pixel2phys(int pixel) const
  {
  return ((double)pixel) * mScale;
  }

SdPoint SdScaler::phys2pixel(SdPoint phys) const
  {
  return SdPoint( phys2pixel(phys.x()), phys2pixel(phys.y()) );
  }

SdPoint SdScaler::pixel2phys(SdPoint pixel) const
  {
  return SdPoint( pixel2phys(pixel.x()), pixel2phys(pixel.y()) );
  }

SdRect SdScaler::phys2pixel(SdRect phys) const
  {
  return SdRect( phys2pixel(phys.left()), phys2pixel(phys.top()), phys2pixel(phys.width()), phys2pixel(phys.height()) );
  }

SdRect SdScaler::pixel2phys(SdRect pixel) const
  {
  return SdRect( pixel2phys(pixel.left()), pixel2phys(pixel.top()), pixel2phys(pixel.width()), pixel2phys(pixel.height()) );
  }


