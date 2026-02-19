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

#include "SdPvAngle.h"
#include "SdUtil.h"





//Разность между двумя углами (угол на который нужно повернуться, чтобы из данного угла получить целевой)
double SdPvAngle::sweep( SdPvAngle sour ) const
  {
  if( mValue > sour.mValue ) return ((sour.mValue - mValue) + 360000) / 1000.0;
  return (sour.mValue - mValue) / 1000.0;
  }





SdPvAngle SdPvAngle::operator += ( SdPvAngle sour ) {
  mValue = norm( mValue + sour.mValue );
  return *this;
  }






// SdPvAngle SdPvAngle::complement() const {
//   return SdPvAngle( 360000 - mValue );
//   }




//Convert angle to string representation
QString SdPvAngle::toString() const
  {
  if( mValue < 0 )
    //For undefined angles
    return QString();
  return SdUtil::log2physStr( mValue, 0.001 );
  }




//Create angle from string representation
SdPvAngle SdPvAngle::fromString(const QString str)
  {
  return SdPvAngle( SdUtil::phys2log( str, 0.001 ) );
  }


