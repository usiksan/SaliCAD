/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/

#include "SdPropPartPin.h"


void SdPropPartPin::operator =(const SdPropPartPin &sour)
  {
  mLayer   = sour.mLayer;
  mPinType = sour.mPinType;
  mSide    = sour.mSide;
  }




void SdPropPartPin::append(const SdPropPartPin &sour)
  {
  mLayer.append( sour.mLayer );
  mPinType.append( sour.mPinType );
  mSide.append( sour.mSide );
  }




void SdPropPartPin::clear()
  {
  mLayer.clear();
  mPinType.clear();
  mSide.clear();
  }




bool SdPropPartPin::match(const SdPropPartPin &prop)
  {
  return mLayer.match(prop.mLayer) && mPinType.match(prop.mPinType) && mSide.match(prop.mSide);
  }



//!
//! \brief json Overloaded function to write object content into json writer
//! \param js   Json writer
//!
void SdPropPartPin::json(SvJsonWriter &js) const
  {
  mLayer.json( QStringLiteral("PartPinLayer"), js );
  mPinType.json( QStringLiteral("PartPinType"), js );
  mSide.jsonStratum( js );
  }




//!
//! \brief json Overloaded function to read object content from json reader
//! \param js   Json reader
//!
void SdPropPartPin::json(const SvJsonReader &js)
  {
  mLayer.json( QStringLiteral("PartPinLayer"), js );
  mPinType.json( QStringLiteral("PartPinType"), js );
  mSide.jsonStratum( js );
  }





void SdPropPartPin::saveState(SdPropPartPinState *dst)
  {
  dst->mLayer   = mLayer.layer();
  dst->mPinType = mPinType.str();
  dst->mSide    = mSide.getValue();
  }




void SdPropPartPin::swapState(SdPropPartPinState *src)
  {
  src->mLayer = mLayer.swap( src->mLayer );
  mPinType.swap( &(src->mPinType) );
  src->mSide = mSide.swap( src->mSide );
  }

