/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Angle in 0.001 degree. It always in range from -179999 to 180000
*/

#ifndef SPvAngle_H
#define SPvAngle_H

#include "SdPvInt.h"

//Orthogonal angles
#define da0        0
#define da90   90000
#define da180 180000
#define da270 270000

class SdPvAngle : public SdPvInt
  {
  public:
    SdPvAngle( int sour = 0 );

    bool      operator != ( const SdPvAngle &v ) const { return mValue != v.mValue; }

    int       positive() const { return mValue < 0 ? mValue + 360000 : mValue; }

    double    getDegree() const { return static_cast<double>(mValue) / 1000.0; }
    //Разность между двумя углами (угол на который нужно повернуться, чтобы из данного угла получить целевой)
    double    sweep( SdPvAngle sour ) const;
    int       operator - ( SdPvAngle sour ) const { return mValue - sour.mValue; } //Разность между двумя углами
    int       operator + ( SdPvAngle sour ) const { return mValue + sour.mValue; } //Сумма двух углов
    SdPvAngle operator += ( SdPvAngle i );                 //Операция "довернуть"
    //SdPvAngle complement() const;                    //Дополнение данного угла до 360 градусов

    //Convert angle to string representation
    QString   toString() const;

    //Create angle from string representation
    static SdPvAngle fromString( const QString str );

  private:
    static int       norm( int v );
  };

#endif // SPvAngle_H
