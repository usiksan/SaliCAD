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
#ifndef SDSCRIPTVALUEFUNFACEELLIPSE_H
#define SDSCRIPTVALUEFUNFACEELLIPSE_H

#include "SdScriptValueFunction.h"

class SdScriptValueFunFaceEllipse : public SdScriptValueFunction
  {
    Sd3dModel *mModel;
  public:
    //                                                                                                  radiusX              radiusY               stepAngle,         matrix
    SdScriptValueFunFaceEllipse( Sd3dModel *model ) : SdScriptValueFunction( SD_SCRIPT_TYPE_FACE, SD_SCRIPT_TYPE_FLOAT, SD_SCRIPT_TYPE_FLOAT, SD_SCRIPT_TYPE_FLOAT, SD_SCRIPT_TYPE_MATRIX ), mModel(model) { }

    // SdM3dValue interface
  public:
    //!
    //! \brief toFace Convert object to list of vertex index which is face region
    //! \return       List of vertex index which is face region
    //!
    virtual Sd3drFace toFace() const override { return mModel->faceEllipse( mParamList[0]->toFloat(), mParamList[1]->toFloat(), mParamList[2]->toFloat(), mParamList[3]->toMatrix() ); }

  };


#endif // SDSCRIPTVALUEFUNFACEELLIPSE_H