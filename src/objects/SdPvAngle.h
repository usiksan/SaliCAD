/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Angle in 0.001 degree. It always in range (-180000, 180000]
*/

#ifndef SPvAngle_H
#define SPvAngle_H

#include "objects/SdPv.h"
#include "SvLib/SvJsonIO.h"

//Orthogonal angles
#define da0        0
#define da90   90000
#define da180 180000
#define da270 270000

class SdPvAngle : public SdPv<int,SdPvAngle>
  {
  public:
    SdPvAngle( int sour = 0 ) : SdPv<int,SdPvAngle>( norm(sour) ) {}

    constexpr int raw() const { return mValue; }

    constexpr int as360() const { return mValue < 0 ? mValue + 360000 : mValue; }

    double    getDegree() const { return static_cast<double>(mValue) / 1000.0; }
    //Разность между двумя углами (угол на который нужно повернуться, чтобы из данного угла получить целевой)
    double    sweep( SdPvAngle sour ) const;
    SdPvAngle operator - ( SdPvAngle sour ) const { return SdPvAngle(mValue - sour.mValue); } //Разность между двумя углами
    SdPvAngle operator + ( SdPvAngle sour ) const { return SdPvAngle(mValue + sour.mValue); } //Сумма двух углов
    SdPvAngle operator += ( SdPvAngle i );                 //Операция "довернуть"
    //SdPvAngle complement() const;                    //Дополнение данного угла до 360 градусов

    //Convert angle to string representation
    QString   toString() const;

    bool      isGrad0() const { return mValue == norm(da0); }
    bool      isGrad90() const { return mValue == norm(da90); }
    bool      isGrad180() const { return mValue == norm(da180); }
    bool      isGrad270() const { return mValue == norm(da270); }

    //!
    //! \brief json Write integer property to JSON writer
    //! \param key JSON key name
    //! \param js  JSON writer object
    //!
    void       json( const QString key, SvJsonWriter &js ) const { js.jsonInt( key, mValue ); }

    //!
    //! \brief json Read integer property from JSON reader
    //! \param key JSON key name
    //! \param js  JSON reader object
    //!
    void       json( const QString key, const SvJsonReader &js ) { js.jsonInt( key, mValue ); mValue = norm(mValue); }

    //Create angle from string representation
    static SdPvAngle fromString( const QString str );

  private:
    static constexpr int norm( int v )
      {
      v %= 360000;
      if( v <= -180000 ) v += 360000;
      if( v > 180000 ) v -= 360000;
      return v;
      }
  };

#endif // SPvAngle_H
