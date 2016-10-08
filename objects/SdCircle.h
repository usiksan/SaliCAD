/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/

#ifndef SDCIRCLE_H
#define SDCIRCLE_H

#include "SdPoint.h"

class SdRect;

struct SdCircle
  {
    SdPoint center;  //Центр окружности
    int     radius;  //Радиус окружности

    SdCircle() : center(), radius(0) {}
    SdCircle( SdPoint c, int r ) : center(c), radius(r) {}

    bool   isPointInside(SdPoint p) const;    //Истина, если точка внутри окружности
    bool   isRectInside( SdRect r ) const;    //Истина, если прямоугольник внутри окружности
    bool   isCircleInside( SdCircle c) const; //Истина, если окружность внутри окружности

    SdRect overRect() const;           //Описанный квадрат
    SdRect innerRect() const;          //Вписанный квадрат
  };

#endif // SDCIRCLE_H
