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
#ifndef SDSCRIPTVALUEFUNLFACEPINCURVETWO_H
#define SDSCRIPTVALUEFUNLFACEPINCURVETWO_H

#include "SdScriptValueFunction.h"

class SdScriptValueFunLFacePinCurveTwo : public SdScriptValueFunction
  {
    Sd3dModel *mModel;
  public:
    SdScriptValueFunLFacePinCurveTwo( Sd3dModel *model ) :
      //                                                Sd3drFace &face,      float firstLen       float middleLen         float lastLen        float radius,         float angleFirst,      float  angleLast,       int sideCount
      SdScriptValueFunction( SD_SCRIPT_TYPE_FACE_LIST, SD_SCRIPT_TYPE_FACE, SD_SCRIPT_TYPE_FLOAT, SD_SCRIPT_TYPE_FLOAT, SD_SCRIPT_TYPE_FLOAT, SD_SCRIPT_TYPE_FLOAT, SD_SCRIPT_TYPE_FLOAT, SD_SCRIPT_TYPE_FLOAT, SD_SCRIPT_TYPE_FLOAT ),
      mModel(model)
      {
      }

    //!
    //! \brief toFaceList Convert object to list of face each of which is list of vertex index
    //! \return           List of face each of which is list of vertex index
    //!
    virtual Sd3drFaceList toFaceList() const override
      {
      return mModel->faceListPinCurveTwo( mParamList[0]->toFace(), mParamList[1]->toFloat(), mParamList[2]->toFloat(), mParamList[3]->toFloat(), mParamList[4]->toFloat(), mParamList[5]->toFloat(), mParamList[6]->toFloat(), mParamList[7]->toFloat() );
      }

  };

#endif // SDSCRIPTVALUEFUNLFACEPINCURVETWO_H
