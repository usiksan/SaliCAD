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
#ifndef SDSCRIPTVALUEFUNLFACEINDEXED_H
#define SDSCRIPTVALUEFUNLFACEINDEXED_H

#include "SdScriptValueFunction.h"

class SdScriptValueFunLFaceIndexed : public SdScriptValueFunction
  {
  public:
    SdScriptValueFunLFaceIndexed() :
      SdScriptValueFunction( SD_SCRIPT_TYPE_FACE_LIST, SD_SCRIPT_TYPE_FACE_LIST, SD_SCRIPT_TYPE_AFLOAT )
      {
      }

    //!
    //! \brief toFaceList Convert object to list of face each of which is list of vertex index
    //! \return           List of face each of which is list of vertex index
    //!
    virtual Sd3drFaceList  toFaceList() const override { return Sd3drModel::faceListIndexed( mParamList[0]->toFaceList(), mParamList[1]->toFloatList() ); }

  };


#endif // SDSCRIPTVALUEFUNLFACEINDEXED_H
