/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Tracing polygon properties. It contains windows gap, net name which polygon and polygon stratum
*/
#include "SdPropPolygon.h"




//!
//! \brief json Overloaded function to write object content into json writer
//! \param js   Json writer
//!
void SdPropPolygon::json(SvJsonWriter &js) const
  {
  mGap.json( QStringLiteral("pGap"), js );
  mStratum.json( QStringLiteral("pStratum"), js );
  mNetName.json( QStringLiteral("pNet"), js );
  }



//!
//! \brief json Function to read object content from json reader
//! \param js   Json reader
//!
void SdPropPolygon::json(const SvJsonReader &js)
  {
  mGap.json( QStringLiteral("pGap"), js );
  mStratum.json( QStringLiteral("pStratum"), js );
  mNetName.json( QStringLiteral("pNet"), js );
  }




//!
//! \brief swap Swap all polygon properties with another instance
//! \param other Other SdPropPolygon object to swap with
//!
void SdPropPolygon::swap(SdPropPolygon &other)
  {
  mGap.swap( other.mGap );
  mNetName.swap( other.mNetName );
  mStratum.swap( other.mStratum );
  }






