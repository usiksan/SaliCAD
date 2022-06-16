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
//! \brief toFaceList Convert object to list of face each of which is list of vertex index
//! \return           List of face each of which is list of vertex index
//!
Sd3drFaceList SdScriptValueArray3dFace::toFaceList() const
  {
  Sd3drFaceList faceList;
  faceList.reserve( mArray.count() );
  for( auto ptr : mArray )
    faceList.append( ptr->toFaceList() );
  return faceList;
  }
