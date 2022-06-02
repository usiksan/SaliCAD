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

  The function builds box model from its size
*/
#ifndef SDSCRIPTVALUEFUNLFACECURVEXZ_H
#define SDSCRIPTVALUEFUNLFACECURVEXZ_H

#include "SdScriptValueFunction.h"

class SdScriptValueFunLFaceCurveXZ : public SdScriptValueFunction
  {
    Sd3dModel *mModel;
  public:
    SdScriptValueFunLFaceCurveXZ( Sd3dModel *model ) :
      //                                                Sd3drFace &face,       float radius,         float angleSrc,       float angleDst,        int sideCount,         bool close
      SdScriptValueFunction( SD_SCRIPT_TYPE_FACE_LIST, SD_SCRIPT_TYPE_FACE, SD_SCRIPT_TYPE_FLOAT, SD_SCRIPT_TYPE_FLOAT, SD_SCRIPT_TYPE_FLOAT, SD_SCRIPT_TYPE_FLOAT, SD_SCRIPT_TYPE_BOOL ),
      mModel(model)
      {
      }

    //!
    //! \brief toFaceList Convert object to list of face each of which is list of vertex index
    //! \return           List of face each of which is list of vertex index
    //!
    virtual Sd3drFaceList toFaceList() const override
      {
      return mModel->faceListCurveXZ( mParamList[0]->toFace(), mParamList[1]->toFloat(), mParamList[2]->toFloat(), mParamList[3]->toFloat(), mParamList[4]->toFloat(), mParamList[5]->toBool() );
      }

  };

#endif // SDSCRIPTVALUEFUNLFACECURVEXZ_H
