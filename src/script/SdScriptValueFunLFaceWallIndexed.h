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

  Embedded function.

  The function builds wall from two faces
*/
#ifndef SDSCRIPTVALUEFUNLFACEWALLINDEXED_H
#define SDSCRIPTVALUEFUNLFACEWALLINDEXED_H

#include "SdScriptValueFunction.h"

class SdScriptValueFunLFaceWallIndexed : public SdScriptValueFunction
  {
  public:
    SdScriptValueFunLFaceWallIndexed() :
      SdScriptValueFunction( SD_SCRIPT_TYPE_FACE_LIST, SD_SCRIPT_TYPE_FACE, SD_SCRIPT_TYPE_FACE, SD_SCRIPT_TYPE_AFLOAT )
      {
      }

    //!
    //! \brief toFaceList Convert object to list of face each of which is list of vertex index
    //! \return           List of face each of which is list of vertex index
    //!
    virtual Sd3drFaceList  toFaceList() const override { return Sd3drModel::faceListWallIndexed( mParamList[0]->toFace(), mParamList[1]->toFace(), mParamList[2]->toFloatList() ); }

  };


#endif // SDSCRIPTVALUEFUNLFACEWALLINDEXED_H
