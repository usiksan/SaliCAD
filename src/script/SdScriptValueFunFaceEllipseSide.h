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

  The function build ellipse region
*/
#ifndef SDSCRIPTVALUEFUNFACEELLIPSESIDE_H
#define SDSCRIPTVALUEFUNFACEELLIPSESIDE_H

#include "SdScriptValueFunction.h"

class SdScriptValueFunFaceEllipseSide : public SdScriptValueFunction
  {
    Sd3drModel *mModel;
  public:
    //                                                                                                  radiusX              radiusY               side count            matrix
    SdScriptValueFunFaceEllipseSide( Sd3drModel *model ) : SdScriptValueFunction( SD_SCRIPT_TYPE_FACE, SD_SCRIPT_TYPE_FLOAT, SD_SCRIPT_TYPE_FLOAT, SD_SCRIPT_TYPE_FLOAT, SD_SCRIPT_TYPE_MATRIX ), mModel(model) { }

    // SdM3dValue interface
  public:
    //!
    //! \brief toFace Convert object to list of vertex index which is face region
    //! \return       List of vertex index which is face region
    //!
    virtual Sd3drFace toFace() const override { return mModel->faceEllipseSide( mParamList[0]->toFloat(), mParamList[1]->toFloat(), static_cast<int>(mParamList[2]->toFloat()), mParamList[3]->toMatrix() ); }

  };


#endif // SDSCRIPTVALUEFUNFACEELLIPSESIDE_H
