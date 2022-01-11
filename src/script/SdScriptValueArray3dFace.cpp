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

  Array of 3d faces.
*/
#include "SdScriptValueArray3dFace.h"



//!
//! \brief toModel Convert object to 3d model
//! \return        3d model
//!
SdScriptVal3dModel SdScriptValueArray3dFace::toModel() const
  {
  SdScriptVal3dModel model;
  for( auto ptr : mArray )
    model.append( ptr->toFace() );
  return model;
  }

