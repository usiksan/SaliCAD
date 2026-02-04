/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/

#include "SdPropSymPin.h"
#include "SdJsonIO.h"


void SdPropSymPin::operator =(const SdPropSymPin &sour)
  {
  mLayer   = sour.mLayer;
  mPinType = sour.mPinType;
  }




void SdPropSymPin::append(const SdPropSymPin &sour)
  {
  mLayer.append( sour.mLayer );
  mPinType.append( sour.mPinType );
  }




void SdPropSymPin::clear()
  {
  mLayer.clear();
  mPinType.clear();
  }




bool SdPropSymPin::match(const SdPropSymPin &prop)
  {
  return mLayer.match(prop.mLayer) && mPinType.match(prop.mPinType);
  }




//!
//! \brief json Function to write object content into json writer
//! \param js   Json writer
//!
void SdPropSymPin::json(SvJsonWriter &js) const
  {
  mLayer.json( QStringLiteral("SymPinLayer"), js );
  mPinType.json( QStringLiteral("SymPinType"), js );
  }



//!
//! \brief json Function to read object content from json reader
//! \param js   Json reader
//!
void SdPropSymPin::json(const SdJsonReader &js)
  {
  mLayer.json( QStringLiteral("SymPinLayer"), js );
  mPinType.json( QStringLiteral("SymPinType"), js );
  }






void SdPropSymPin::saveState(SdPropSymPinState *dst)
  {
  dst->mPinType = mPinType.getValue();
  dst->mLayer   = mLayer.layer();
  }




void SdPropSymPin::swapState(SdPropSymPinState *src)
  {
  src->mPinType = mPinType.swap( src->mPinType );
  src->mLayer   = mLayer.swap( src->mLayer );
  }


