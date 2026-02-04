/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/
#include "SdTextImplement.h"
#include "SdJsonIO.h"


//!
//! \brief json Overloaded function to write object content into json writer
//! \param js   Json writer
//!
void SdTextImplement::json(const QString &prefix, SvJsonWriter &js) const
  {
  mProp.json( prefix, js );
  js.jsonPoint( prefix + QString("org"), mOrigin );
  }




//!
//! \brief json Overloaded function to read object content from json reader
//! \param js   Json reader
//!
void SdTextImplement::json(const QString &prefix, const SdJsonReader &js)
  {
  mProp.json( prefix, js );
  js.jsonPoint( prefix + QString("org"), mOrigin );
  }
