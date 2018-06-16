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
#include "SdEnvir.h"

SdStratum::SdStratum() :
  SdPropInt()
  {

  }


SdStratum::SdStratum(int str) :
  SdPropInt(str)
  {

  }




//Write-read stratum
void SdStratum::writeStratum(QJsonObject &obj) const
  {
  obj.insert( QStringLiteral("Stratum"), QString::number( mValue, 16 ) );
  }




void SdStratum::readStratum(const QJsonObject &obj)
  {
  mValue = obj.value( QStringLiteral("Stratum") ).toString().toInt( nullptr, 16 );
  }




//Flip stratum stack for pcb bottom and unchange it if other
int SdStratum::stratum(bool top) const
  {
  if( mValue < 0 ) return 0;
  if( !top && mValue == stmTop )
    return stmBottom;
  return mValue;
  }




//Flip stratum stack if component is bottom and unchange it if none
int SdStratum::stratumComp(const SdStratum &src) const
  {
  return stratum( !src.isBottom() );
  }



//Return layer associated with this stratum and trace variant
SdLayer *SdStratum::layer(SdLayerTrace trace) const
  {
  //sdEnvir
  }



//Get stratum index of first stratum in the stack
int SdStratum::stratumIndex(int stratum)
  {
  for( int i = 0; i < 30; i++ )
    if( stratum & (1 << i) ) return i;
  return 0;
  }
