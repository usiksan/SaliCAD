/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Script language is C-style hi-level language for programming 3d models, 2d parametric graphics and
  schematic calculations.

  Array of floats
*/
#include "SdScriptValueArrayFloat.h"




//!
//! \brief toIndex Convert object to float list
//! \return        Float list
//!
QList<float> SdScriptValueArrayFloat::toFloatList() const
  {
  QList<float> list;
  list.reserve( mArray.count() );
  for( auto ptr : mArray )
    list.append( ptr->toFloat() );
  return list;
  }
