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

  The function builds rectangle region with center in 0
*/
#ifndef SDSCRIPTVALUEFUNFACERECTSIDE_H
#define SDSCRIPTVALUEFUNFACERECTSIDE_H

#include "SdScriptValueFunction.h"

class SdScriptValueFunFaceRectSide : public SdScriptValueFunction
  {
    Sd3drModel *mModel;
  public:
    //                                                                                                rect lenght          rect width           side count             matrix
    SdScriptValueFunFaceRectSide( Sd3drModel *model ) : SdScriptValueFunction( SD_SCRIPT_TYPE_FACE, SD_SCRIPT_TYPE_FLOAT, SD_SCRIPT_TYPE_FLOAT, SD_SCRIPT_TYPE_FLOAT, SD_SCRIPT_TYPE_MATRIX ), mModel(model) { }

    // SdM3dValue interface
  public:
    //!
    //! \brief toFace Convert object to list of vertex index which is face region
    //! \return       List of vertex index which is face region
    //!
    virtual Sd3drFace toFace() const override { return mModel->faceRectangleSide( mParamList[0]->toFloat(), mParamList[1]->toFloat(), mParamList[2]->toFloat(), mParamList[3]->toMatrix() ); }

  };

#endif // SDSCRIPTVALUEFUNFACERECTSIDE_H
