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

#include "SdPropAngle.h"
#include "SdUtil.h"


SdPropAngle::SdPropAngle(int sour)
  {
  sour %= 360000;                //Преобразовать чтобы находился в диапазоне
  if( sour < 0 ) sour += 360000; //Преобразовать в положительные углы
  mValue = sour;
  }




//Разность между двумя углами (угол на который нужно повернуться, чтобы из данного угла получить целевой)
double SdPropAngle::sweep( SdPropAngle sour ) const
  {
  if( mValue > sour.mValue ) return ((sour.mValue - mValue) + 360000) / 1000.0;
  return (sour.mValue - mValue) / 1000.0;
  }




SdPropAngle SdPropAngle::operator -(SdPropAngle sour) const
  {
  SdPropAngle an;
  an.mValue = mValue - sour.mValue;
  if( an.mValue < 0 ) an.mValue += 360000;
  return an;
  }




SdPropAngle SdPropAngle::operator + ( SdPropAngle sour ) const {
  SdPropAngle an;
  an.mValue = mValue + sour.mValue;
  if( an.mValue < 0 ) an.mValue += 360000;
  if( an.mValue >= 360000 ) an.mValue %= 360000;
  return an;
  }




SdPropAngle SdPropAngle::operator += ( int sour ) {
  mValue += sour;
  while( mValue >= 360000 ) mValue -= 360000;
  while( mValue < 0 )       mValue += 360000;
  return *this;
  }




void SdPropAngle::append( SdPropAngle an ) {
  if( mValue == NoValue ) mValue = an.mValue;
  else if( mValue >= 0 && mValue != an.mValue ) mValue = AllValue;
  }




SdPropAngle SdPropAngle::complement() const {
  return SdPropAngle( 360000 - mValue );
  }




//Convert angle to string representation
QString SdPropAngle::toString() const
  {
  if( mValue < 0 )
    //For undefined angles
    return QString();
  return SdUtil::log2physStr( mValue, 0.001 );
  }




//Create angle from string representation
SdPropAngle SdPropAngle::fromString(const QString str)
  {
  return SdPropAngle( SdUtil::phys2log( str, 0.001 ) );
  }

