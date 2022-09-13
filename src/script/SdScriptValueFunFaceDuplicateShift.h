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

  The function duplicate face with conversion matrix
*/
#ifndef SDSCRIPTVALUEFUNFACEDUPLICATESHIFT_H
#define SDSCRIPTVALUEFUNFACEDUPLICATESHIFT_H

#include "SdScriptValueFunction.h"

class SdScriptValueFunFaceDuplicateShift : public SdScriptValueFunction
  {
    Sd3drModel *mModel;
  public:
    SdScriptValueFunFaceDuplicateShift( Sd3drModel *model ) : SdScriptValueFunction( SD_SCRIPT_TYPE_FACE, SD_SCRIPT_TYPE_FACE, SD_SCRIPT_TYPE_FLOAT ), mModel(model) { }

    // SdM3dValue interface
  public:
    //!
    //! \brief toFace Convert object to list of vertex index which is face region
    //! \return       List of vertex index which is face region
    //!
    virtual Sd3drFace toFace() const override { return mModel->faceDuplicateShift( mParamList[0]->toFace(), mParamList[1]->toFloat() ); }
  };


#endif // SDSCRIPTVALUEFUNFACEDUPLICATESHIFT_H
