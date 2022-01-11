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

  Array of 3d segments.
*/
#include "SdScriptValueArray3dSegment.h"



//!
//! \brief toPath Convert object to path of 3d points
//! \return       Path of 3d points
//!
SdScriptVal3dPath SdScriptValueArray3dSegment::toPath() const
  {
  SdScriptVal3dPath path;
  for( auto ptr : mArray )
    path.append( ptr->toSegment() );
  return path;
  }


