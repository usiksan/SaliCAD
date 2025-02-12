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
#ifndef SDSCRIPTVALUEFUNFACECURVEXZ_H
#define SDSCRIPTVALUEFUNFACECURVEXZ_H

#include "SdScriptValueFunction.h"

class SdScriptValueFunFaceCurveXZ : public SdScriptValueFunction
  {
    Sd3drModel *mModel;
  public:

    SdScriptValueFunFaceCurveXZ( Sd3drModel *model ) :
      //                                        const Sd3drFace &face, float radius,            float angleSrc,        float angleDst
      SdScriptValueFunction( SD_SCRIPT_TYPE_FACE, SD_SCRIPT_TYPE_FACE, SD_SCRIPT_TYPE_FLOAT, SD_SCRIPT_TYPE_FLOAT, SD_SCRIPT_TYPE_FLOAT ),
      mModel(model) { }

    // SdM3dValue interface
  public:
    //!
    //! \brief toFace Convert object to list of vertex index which is face region
    //! \return       List of vertex index which is face region
    //!
    virtual Sd3drFace toFace() const override { return mModel->faceCurveXZ( mParamList[0]->toFace(), mParamList[1]->toFloat(), mParamList[2]->toFloat(), mParamList[3]->toFloat() ); }

  };

#endif // SDSCRIPTVALUEFUNFACECURVEXZ_H
