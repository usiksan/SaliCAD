/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/
#include "SdStratum.h"

SdStratum::SdStratum() :
  SdPropInt()
  {

  }


SdStratum::SdStratum(int str) :
  SdPropInt(str)
  {

  }




void SdStratum::writeStratum(QJsonObject &obj) const
  {
  obj.insert( QStringLiteral("Stratum"), QString::number( mValue, 16 ) );
  }




void SdStratum::readStratum(const QJsonObject &obj)
  {
  mValue = obj.value( QStringLiteral("Stratum") ).toString().toInt( nullptr, 16 );
  }




int SdStratum::stratum(bool top) const
  {
  if( mValue < 0 ) return 0;
  if( !top && mValue == stmTop )
    return stmBottom;
  return mValue;
  }




int SdStratum::stratumComp(const SdStratum &src) const
  {
  return stratum( !src.isBottom() );
  }
