/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Tracing road properties. It contains road width, net name which road and road stratum
*/

#include "SdPropRoad.h"




//!
//! \brief json Overloaded function to write object content into json writer
//! \param js   Json writer
//!
void SdPropRoad::json(SvJsonWriter &js) const
  {
  mWidth.json( QStringLiteral("rWidth"), js );
  mStratum.json( QStringLiteral("rStratum"), js );
  mNetName.json( QStringLiteral("rNet"), js );
  }




//!
//! \brief json Overloaded function to read object content from json reader
//! \param js   Json reader
//!
void SdPropRoad::json(const SvJsonReader &js)
  {
  mWidth.json( QStringLiteral("rWidth"), js );
  mStratum.json( QStringLiteral("rStratum"), js );
  mNetName.json( QStringLiteral("rNet"), js );
  }





//!
//! \brief swap Swap all road properties with another instance
//! \param other Other SdPropRoad object to swap with
//!
void SdPropRoad::swap(SdPropRoad &other)
  {
  mWidth.swap( other.mWidth );
  mNetName.swap( other.mNetName );
  mStratum.swap( other.mStratum );
  }


