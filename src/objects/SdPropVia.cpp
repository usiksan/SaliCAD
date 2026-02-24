/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Tracing via properties. It contains via pad type, net name which via and via stratum
*/
#include "SdPropVia.h"




//!
//! \brief json Overloaded function to write object content into json writer
//! \param js   Json writer
//!
void SdPropVia::json(SvJsonWriter &js) const
  {
  mStratum.json( QStringLiteral("vStratum"), js );
  mNetName.json( QStringLiteral("vNet"), js );
  mPadType.json( QStringLiteral("vPad"), js );
  }




//!
//! \brief json Overloaded function to read object content from json reader
//! \param js   Json reader
//!
void SdPropVia::json(const SdJsonReader &js)
  {
  mStratum.json( QStringLiteral("vStratum"), js );
  mNetName.json( QStringLiteral("vNet"), js );
  mPadType.json( QStringLiteral("vPad"), js );
  }




//!
//! \brief swap Swap all via properties with another instance
//! \param other Other SdPropVia object to swap with
//!
void SdPropVia::swap(SdPropVia &other)
  {
  mStratum.swap( other.mStratum );
  mPadType.swap( other.mPadType );
  mNetName.swap( other.mNetName );
  }

