/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Part implementation properties
*/

#include "SdPropPartImp.h"
#include "SdJsonIO.h"



//!
//! \brief json Function to write object content into json writer
//! \param js   Json writer
//!
void SdPropPartImp::json(SvJsonWriter &js) const
  {
  mAngle.json( QStringLiteral("PartImpAngle"), js );
  mSide.jsonSide( js );
  }



//!
//! \brief json Function to read object content from json reader
//! \param js   Json reader
//!
void SdPropPartImp::json(const SdJsonReader &js)
  {
  mAngle.json( QStringLiteral("PartImpAngle"), js );
  mSide.jsonSide( js );
  }




//!
//! \brief swap Swap all part implementation properties with another instance
//! \param other Other SdPropPartImp object to swap with
//!
void SdPropPartImp::swap(SdPropPartImp &other)
  {
  mAngle.swap( other.mAngle );
  mSide.swap( other.mSide );
  }


