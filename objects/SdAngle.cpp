/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/

#include "SdAngle.h"



SdAngle::SdAngle(int sour)
  {
  sour %= 360000;                //Преобразовать чтобы находился в диапазоне
  if( sour < 0 ) sour += 360000; //Преобразовать в положительные углы
  mValue = sour;
  }

//Разность между двумя углами (угол на который нужно повернуться, чтобы из данного угла получить целевой)
double SdAngle::sweep( SdAngle sour ) const
  {
  if( mValue > sour.mValue ) return ((sour.mValue - mValue) + 360000) / 1000.0;
  return (sour.mValue - mValue) / 1000.0;
  }

SdAngle SdAngle::operator -(SdAngle sour) const
  {
  SdAngle an;
  an.mValue = mValue - sour.mValue;
  if( an.mValue < 0 ) an.mValue += 360000;
  return an;
  }

SdAngle SdAngle::operator + ( SdAngle sour ) const {
  SdAngle an;
  an.mValue = mValue + sour.mValue;
  if( an.mValue < 0 ) an.mValue += 360000;
  if( an.mValue >= 360000 ) an.mValue %= 360000;
  return an;
  }

SdAngle SdAngle::operator += ( int sour ) {
  mValue += sour;
  while( mValue >= 360000 ) mValue -= 360000;
  while( mValue < 0 )       mValue += 360000;
  return *this;
  }

void SdAngle::append( SdAngle an ) {
  if( mValue == NoValue ) mValue = an.mValue;
  else if( mValue >= 0 && mValue != an.mValue ) mValue = AllValue;
  }


SdAngle SdAngle::complement() const {
  return SdAngle( 360000 - mValue );
  }

