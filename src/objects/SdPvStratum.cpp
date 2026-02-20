/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/
#include "SdPvStratum.h"
#include "SdJsonIO.h"

#include <algorithm>



//!
//! \brief stratumBuild  Build stratum from one stratum to another
//! \param from          Source stratum
//! \param to            Destignation stratum
//! \param pcbLayerCount Count of pcb signal layers
//! \param rule          Rule for building, one of stmRuleXXX
//!
void SdPvStratum::stratumBuild(SdPvStratum from, SdPvStratum to, int pcbLayerCount, int rule)
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
void SdPvStratum::json(SvJsonWriter &js) const
  {
  json( QStringLiteral("Stratum"), js );
  }




void SdPvStratum::json(const SvJsonReader &js)
  {
  json( QStringLiteral("Stratum"), js );
  }



void SdPvStratum::jsonSide(SvJsonWriter &js) const
  {
  QString side;
  switch( mValue ) {
    case stmTop : side = "top"; break;
    case stmBottom : side = "bot"; break;
    default: side = QString::number( mValue, 16 );
    }
  js.jsonString( "Side", side );
  }



void SdPvStratum::jsonSide(const SvJsonReader &js)
  {
  QString side;
  js.jsonString( "Side", side );
  if( side == "top" ) mValue = stmTop;
  else if( side == "bot" ) mValue = stmBottom;
  else mValue = side.toInt( nullptr, 16 );
  }













//Flip stratum stack for pcb bottom and unchange it if other
SdPvStratum SdPvStratum::stratum(bool top) const
  {
  if( mValue < 0 ) return 0;
  if( !top && mValue == stmTop )
    return stmBottom;
  return mValue;
  }



SdPvStratum SdPvStratum::first() const
  {
  for( int i = 0; i < stmCountMax; ++i )
    if( mValue & (1 << i) ) return SdPvStratum(1 << i);
  return *this;
  }




//Flip stratum stack if component is bottom and unchange it if none
SdPvStratum SdPvStratum::stratumComp(const SdPvStratum &src) const
  {
  return stratum( !src.isBottom() );
  }





//Get first stratum from stratum stack
SdPvStratum SdPvStratum::stratumFirst(SdPvStratum s) const
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
SdPvStratum SdPvStratum::stratumNext(SdPvStratum s) const
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



int SdPvStratum::getStratumIndex() const
  {
  return stratumIndex(mValue);
  }





//Get stratum index of first stratum in the stack
int SdPvStratum::stratumIndex(SdPvStratum stratum)
  {
  //Find first non zero bit, return its index
  for( int i = 0; i < stmCountMax; i++ )
    if( stratum.mValue & (1 << i) ) return i;
  return 0;
  }




//Build stratum stack with stratum count
SdPvStratum SdPvStratum::stratumStack(int stratumCount)
  {
  static const SdPvStratum map[16] = { stmTop,     stmLayer2,  stmLayer4,  stmLayer6,  stmLayer8,  stmLayer10, stmLayer12, stmLayer14,
                                     stmLayer16, stmLayer18, stmLayer20, stmLayer22, stmLayer24, stmLayer26, stmLayer28, stmLayer30 };
  return map[ qBound( 2, stratumCount, 30 ) >> 1 ];
  }


