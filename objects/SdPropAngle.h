/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Angle in 0.001 degree. It always positive and in range from 0 to 360000
*/

#ifndef SSdAngle_H
#define SSdAngle_H

#include "SdPropInt.h"

//Orthogonal angles
#define da0        0
#define da90   90000
#define da180 180000
#define da270 270000

class SdPropAngle : public SdPropInt
  {
  public:
    SdPropAngle() : SdPropInt() {}
    SdPropAngle( int sour );

    double      getDegree() const { return ((double)mValue) / 1000.0; }
    //Разность между двумя углами (угол на который нужно повернуться, чтобы из данного угла получить целевой)
    double      sweep( SdPropAngle sour ) const;
    SdPropAngle operator - ( SdPropAngle sour ) const; //Разность между двумя углами
    SdPropAngle operator + ( SdPropAngle sour ) const; //Сумма двух углов
    SdPropAngle operator += ( int i );                 //Операция "довернуть"
    void        append( SdPropAngle an );              //Добавить значение
    SdPropAngle complement() const;                    //Дополнение данного угла до 360 градусов

  };

#endif // SSdAngle_H
