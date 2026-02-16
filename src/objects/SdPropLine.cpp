/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/
#include "SdJsonIO.h"
#include "SdPropLine.h"
#include "SdUndo.h"





//!
//! \brief json Write line properties to JSON with specified prefix
//! \param prefix JSON key prefix for grouped properties
//! \param js     JSON writer object
//!
void SdPropLine::json(const QString &prefix, SvJsonWriter &js) const
  {
  mWidth.json( prefix + QStringLiteral("Width"), js );
  mType.json( prefix + QStringLiteral("Type"), js );
  mLayer.json( prefix + QStringLiteral("Layer"), js );
  }




//!
//! \brief json Read line properties from JSON with specified prefix
//! \param prefix JSON key prefix for grouped properties
//! \param js     JSON reader object
//!
void SdPropLine::json(const QString &prefix, const SdJsonReader &js)
  {
  mWidth.json( prefix + QStringLiteral("Width"), js );
  mType.json( prefix + QStringLiteral("Type"), js );
  mLayer.json( prefix + QStringLiteral("Layer"), js );
  }




//!
//! \brief swap Swap all line properties with another instance
//! \param other Other SdPropLine object to swap with
//!
void SdPropLine::swap(SdPropLine &other)
  {
  mWidth.swap( other.mWidth );
  mType.swap( other.mType );
  mLayer.swap( other.mLayer );
  }




