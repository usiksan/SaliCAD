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

  Array of 3d vertex.
*/
#include "SdScriptValueArray3dVertex.h"




//!
//! \brief toFace Convert object to list of vertex index which is face region
//! \return       List of vertex index which is face region
//!
Sd3drFace SdScriptValueArray3dVertex::toFace() const
  {
  Sd3drFace face;
  for( auto const ptr : mArray )
    face.append( ptr->toVertexIndex() );
  return face;
  }
