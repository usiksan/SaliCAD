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

  The function build circle region
*/
#ifndef SDSCRIPTVALUEFUNFACECIRCLESIDE_H
#define SDSCRIPTVALUEFUNFACECIRCLESIDE_H

#include "SdScriptValueFunction.h"

class SdScriptValueFunFaceCircleSide : public SdScriptValueFunction
  {
    Sd3drModel *mModel;
  public:
    SdScriptValueFunFaceCircleSide( Sd3drModel *model ) : SdScriptValueFunction( SD_SCRIPT_TYPE_FACE, SD_SCRIPT_TYPE_FLOAT, SD_SCRIPT_TYPE_FLOAT, SD_SCRIPT_TYPE_MATRIX ), mModel(model) { }

    // SdM3dValue interface
  public:
    //!
    //! \brief toFace Convert object to list of vertex index which is face region
    //! \return       List of vertex index which is face region
    //!
    virtual Sd3drFace toFace() const override { return mModel->faceCircleSide( mParamList[0]->toFloat(), static_cast<int>(mParamList[1]->toFloat()), mParamList[2]->toMatrix() ); }

  };


#endif // SDSCRIPTVALUEFUNFACECIRCLESIDE_H
