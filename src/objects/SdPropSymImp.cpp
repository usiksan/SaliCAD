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


void SdPropSymImp::operator =(const SdPropSymImp &sour)
  {
  mAngle  = sour.mAngle;
  mMirror = sour.mMirror;
  }




void SdPropSymImp::append(const SdPropSymImp &sour)
  {
  mAngle.append( sour.mAngle );
  mMirror.append( sour.mMirror );
  }




void SdPropSymImp::clear()
  {
  mAngle.clear();
  mMirror.clear();
  }




bool SdPropSymImp::match(const SdPropSymImp &prop)
  {
  return mAngle.match( prop.mAngle ) && mMirror.match( prop.mMirror );
  }



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









void SdPropSymImp::saveState(SdPropSymImpState *dst)
  {
  dst->mAngle  = mAngle.getValue();
  dst->mMirror = mMirror.getValue();
  }




void SdPropSymImp::swapState(SdPropSymImpState *src)
  {
  src->mAngle  = mAngle.swap( src->mAngle );
  src->mMirror = mMirror.swap( src->mMirror );
  }

