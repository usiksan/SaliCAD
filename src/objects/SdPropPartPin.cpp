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
#include "SdJsonIO.h"



//!
//! \brief json Overloaded function to write object content into json writer
//! \param js   Json writer
//!
void SdPropPartPin::json(SvJsonWriter &js) const
  {
  mLayer.json( QStringLiteral("PartPinLayer"), js );
  mPinType.json( QStringLiteral("PartPinType"), js );
  mSide.json( js );
  }




//!
//! \brief json Overloaded function to read object content from json reader
//! \param js   Json reader
//!
void SdPropPartPin::json(const SdJsonReader &js)
  {
  mLayer.json( QStringLiteral("PartPinLayer"), js );
  mPinType.json( QStringLiteral("PartPinType"), js );
  mSide.json( js );
  }



//!
//! \brief swap Swap all part pin properties with another instance
//! \param other Other SdPropPartPin object to swap with
//!
void SdPropPartPin::swap(SdPropPartPin &other)
  {
  mLayer.swap( other.mLayer );
  mSide.swap( other.mSide );
  mPinType.swap( other.mPinType );
  }




