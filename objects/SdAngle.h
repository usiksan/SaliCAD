/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/

#ifndef SSdAngle_H
#define SSdAngle_H

#include "SdIntProp.h"

class SdAngle : public SdIntProp
  {
  public:
    SdAngle() : SdIntProp() {}
    SdAngle( int sour );

    double  getDegree() const { return ((double)mValue) / 1000.0; }
    //Разность между двумя углами (угол на который нужно повернуться, чтобы из данного угла получить целевой)
    double  sweep( SdAngle sour ) const;
    SdAngle operator - ( SdAngle sour ) const; //Разность между двумя углами
    SdAngle operator + ( SdAngle sour ) const; //Сумма двух углов
    SdAngle operator += ( int i );             //Операция "довернуть"
    void    append( SdAngle an );              //Добавить значение
    SdAngle complement() const;                //Дополнение данного угла до 360 градусов

  };

#endif // SSdAngle_H
