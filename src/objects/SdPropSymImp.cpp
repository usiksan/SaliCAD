/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Symbol implementation properties:
   angle of implementation rotation
   mirror of implementation
*/

#include "SdPropSymImp.h"





//!
//! \brief json Overloaded function to write object content into json writer
//! \param js   Json writer
//!
void SdPropSymImp::json(SvJsonWriter &js) const
  {
  mAngle.json( QStringLiteral("SymImpAngle"), js );
  mMirror.json( QStringLiteral("SymImpMirror"), js );
  }



//!
//! \brief json Overloaded function to read object content from json reader
//! \param js   Json reader
//!
void SdPropSymImp::json(const SvJsonReader &js)
  {
  mAngle.json( QStringLiteral("SymImpAngle"), js );
  mMirror.json( QStringLiteral("SymImpMirror"), js );
  }




//!
//! \brief swap Swap all symbol implementation properties with another instance
//! \param other Other SdPropSymImp object to swap with
//!
void SdPropSymImp::swap(SdPropSymImp &other)
  {
  mAngle.swap( other.mAngle );
  mMirror.swap( other.mMirror );
  }


