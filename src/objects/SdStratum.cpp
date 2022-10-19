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
#include "SdJsonIO.h"

#include <algorithm>

SdStratum::SdStratum() :
  SdPropInt()
  {

  }


SdStratum::SdStratum(int str) :
  SdPropInt(str)
  {

  }



//!
//! \brief stratumBuild  Build stratum from one stratum to another
//! \param from          Source stratum
//! \param to            Destignation stratum
//! \param pcbLayerCount Count of pcb signal layers
//! \param rule          Rule for building, one of stmRuleXXX
//!
void SdStratum::stratumBuild(SdStratum from, SdStratum to, int pcbLayerCount, int rule)
  {
  int indexFrom = from.getStratumIndex();
  int indexTo   = to.getStratumIndex();
  if( indexFrom == indexTo ) {
    mValue = from.mValue;
    return;
    }
  if( indexFrom > indexTo )
    std::swap( indexFrom, indexTo );

  if( (indexFrom == 0 && indexTo == 1) || (indexFrom == (pcbLayerCount - 2) && indexTo == 29) ) {
    //From outer layer to nearest inner
    if( (rule & stmRuleBlind) == 0 ) {
      //Blind holes disabled convert to throught
      indexFrom = 0;
      indexTo = 29;
      }
    }
  else if( indexFrom == 0 || indexTo == 29 ) {
    //From outer layer to deep inner
    //convert to throught
    indexFrom = 0;
    indexTo = 29;
    }
  else if( (indexFrom & 1) == 0 || (indexFrom + 1) != indexTo ) {
    //Beatween internal planes
    if( (rule & stmRuleCore) == 0 ) {
      //Core holes disabled, convert to throught
      indexFrom = 0;
      indexTo = 29;
      }
    else {
      //Convert to core
      indexFrom = 1;
      indexTo = 28;
      }
    }
  else {
    //Beatween internal layers on single plane
    if( (rule & stmRuleHided) == 0 ) {
      if( (rule & stmRuleCore) == 0 ) {
        //Core holes disabled, convert to throught
        indexFrom = 0;
        indexTo = 29;
        }
      else {
        //Convert to core
        indexFrom = 1;
        indexTo = 28;
        }
      }
    }

  mValue = 0;
  for( int i = indexFrom; i <= indexTo; i++ )
    mValue |= 1 << i;

  }



//Write-read stratum
void SdStratum::jsonStratum(SvJsonWriter &js) const
  {
  js.object().insert( QStringLiteral("Stratum"), QString::number( mValue, 16 ) );
  }




void SdStratum::jsonStratum(const SdJsonReader &js)
  {
  mValue = js.object().value( QStringLiteral("Stratum") ).toString().toInt( nullptr, 16 );
  if( js.property()->mVersion == SD_BASE_VERSION_1 ) {
    //Convert bottom stratum
    if( mValue == stmBottomV1 )
      mValue = stmBottom;
    else if( mValue > stmBottomV1 && mValue != stmThrough )
      mValue >>= 1;
    }
  }



void SdStratum::jsonSide(SvJsonWriter &js) const
  {
  QString side;
  switch( mValue ) {
    case stmTop : side = "top"; break;
    case stmBottom : side = "bot"; break;
    default: side = QString::number( mValue, 16 );
    }
  js.jsonString( "Side", side );
  }



void SdStratum::jsonSide(const SdJsonReader &js)
  {
  QString side;
  js.jsonString( "Side", side );
  if( side == "top" ) mValue = stmTop;
  else if( side == "bot" ) mValue = stmBottom;
  else mValue = side.toInt( nullptr, 16 );
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





//Get first stratum from stratum stack
int SdStratum::stratumFirst(SdStratum s) const
  {
  if( mValue <= 0 || s.mValue <= 0 )
    return stmTop;
  int st = 1 << s.getStratumIndex();
  while( st < stmEnd ) {
    if( mValue & st ) return st;
    st <<= 1;
    }
  int end = 1 << s.getStratumIndex();
  st = 1;
  while( st < end ) {
    if( mValue & st ) return st;
    st <<= 1;
    }
  return stmTop;
  }




//Get next stratum from stratum stack
int SdStratum::stratumNext(SdStratum s) const
  {
  if( mValue <= 0 || s.mValue <= 0 )
    return stmTop;
  int st = 1 << (s.getStratumIndex() + 1);
  while( st < stmEnd ) {
    if( mValue & st ) return st;
    st <<= 1;
    }
  int end = 1 << s.getStratumIndex();
  st = 1;
  while( st <= end ) {
    if( mValue & st ) return st;
    st <<= 1;
    }
  return stmTop;
  }



int SdStratum::getStratumIndex() const
  {
  return stratumIndex(mValue);
  }





//Get stratum index of first stratum in the stack
int SdStratum::stratumIndex(int stratum)
  {
  for( int i = 0; i < 30; i++ )
    if( stratum & (1 << i) ) return i;
  return 0;
  }




//Build stratum stack with stratum count
int SdStratum::stratumStack(int stratumCount)
  {
  switch( stratumCount ) {
    case 30 : return stmLayer30;
    case 28 : return stmLayer28;
    case 26 : return stmLayer26;
    case 24 : return stmLayer24;
    case 22 : return stmLayer22;
    case 20 : return stmLayer20;
    case 18 : return stmLayer18;
    case 16 : return stmLayer16;
    case 14 : return stmLayer14;
    case 12 : return stmLayer12;
    case 10 : return stmLayer10;
    case 8 : return stmLayer8;
    case 6 : return stmLayer6;
    case 4 : return stmLayer4;
    case 2 : return stmLayer2;
    case 1 : return stmLayer1;
    default:
      return stmTop;
    }
  }
