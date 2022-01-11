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
//! \brief toRegion Convert object to 3d region which is closed region of 3d vertex
//! \return         3d region which is closed region of 3d vertex
//!
SdScriptVal3dRegion SdScriptValueArray3dVertex::toRegion() const
  {
  SdScriptVal3dRegion region;
  for( auto const ptr : mArray )
    region.append( ptr->toVertex() );
  return region;
  }


