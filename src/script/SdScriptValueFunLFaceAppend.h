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

  The function append single face to face list
*/
#ifndef SDSCRIPTVALUEFUNLFACEAPPEND_H
#define SDSCRIPTVALUEFUNLFACEAPPEND_H

#include "SdScriptValueFunction.h"

class SdScriptValueFunLFaceAppend : public SdScriptValueFunction
  {
  public:
    SdScriptValueFunLFaceAppend() :
      SdScriptValueFunction( SD_SCRIPT_TYPE_FACE_LIST, SD_SCRIPT_TYPE_FACE_LIST, SD_SCRIPT_TYPE_FACE )
      {

      }


    //!
    //! \brief toFaceList Convert object to list of face each of which is list of vertex index
    //! \return           List of face each of which is list of vertex index
    //!
    virtual Sd3drFaceList toFaceList() const override { return mParamList[0]->toFaceList() << mParamList[1]->toFace(); }

  };

#endif // SDSCRIPTVALUEFUNLFACEAPPEND_H
