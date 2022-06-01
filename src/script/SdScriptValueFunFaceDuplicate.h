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
#ifndef SDSCRIPTVALUEFUNFACEDUPLICATE_H
#define SDSCRIPTVALUEFUNFACEDUPLICATE_H

#include "SdScriptValueFunction.h"

class SdScriptValueFunFaceDuplicate : public SdScriptValueFunction
  {
    Sd3dModel *mModel;
  public:
    SdScriptValueFunFaceDuplicate( Sd3dModel *model ) : SdScriptValueFunction( SD_SCRIPT_TYPE_FACE, SD_SCRIPT_TYPE_FACE, SD_SCRIPT_TYPE_MATRIX ), mModel(model) { }

    // SdM3dValue interface
  public:
    //!
    //! \brief toFace Convert object to list of vertex index which is face region
    //! \return       List of vertex index which is face region
    //!
    virtual Sd3drFace toFace() const override { return mModel->faceDuplicate( mParamList[0]->toFace(), mParamList[1]->toMatrix() ); }
  };

#endif // SDSCRIPTVALUEFUNFACEDUPLICATE_H
