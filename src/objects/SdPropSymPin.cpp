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




//!
//! \brief swap Swap all symbol pin properties with another instance
//! \param other Other SdPropSymPin object to swap with
//!
void SdPropSymPin::swap(SdPropSymPin &other)
  {
  mLayer.swap( other.mLayer );
  mPinType.swap( other.mPinType );
  }




