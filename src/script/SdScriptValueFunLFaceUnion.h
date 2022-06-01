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

  The function extrudes the model from the region in the direction of the vector
*/

#ifndef SDSCRIPTVALUEFUNLFACEUNION_H
#define SDSCRIPTVALUEFUNLFACEUNION_H

#include "SdScriptValueFunction.h"

class SdM3dFunLFaceUnion : public SdScriptValueFunction
  {
  public:
    SdM3dFunLFaceUnion() :
      SdScriptValueFunction( SD_SCRIPT_TYPE_FACE_LIST, SD_SCRIPT_TYPE_FACE_LIST, SD_SCRIPT_TYPE_FACE_LIST )
      {
      }

    //!
    //! \brief toFaceList Convert object to list of face each of which is list of vertex index
    //! \return           List of face each of which is list of vertex index
    //!
    virtual Sd3drFaceList toFaceList() const override { return mParamList[0]->toFaceList() + mParamList[1]->toFaceList(); }

  };

#endif // SDSCRIPTVALUEFUNLFACEUNION_H
