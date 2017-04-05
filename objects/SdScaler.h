/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/

#ifndef SDSCALER_H
#define SDSCALER_H

#include "SdPoint.h"
#include "SdRect.h"

class SdScaler
  {
  protected:
    double mScale; //Scale factor
  public:
    SdScaler( double scale = 1.0 );

    double  getScale() const { return mScale; }
    void    scale( double step) { mScale *= step; }

    int     phys2pixel( int phys ) const;
    int     pixel2phys( int pixel ) const;

    SdPoint phys2pixel( SdPoint phys ) const;
    SdPoint pixel2phys( SdPoint pixel ) const;

    SdRect  phys2pixel( SdRect phys ) const;
    SdRect  pixel2phys( SdRect pixel ) const;
  };

#endif // SDSCALER_H
