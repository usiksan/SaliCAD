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

  The function builds model of tqfp pin
*/
#ifndef SDSCRIPTVALUEFUNMODELPINTQFP_H
#define SDSCRIPTVALUEFUNMODELPINTQFP_H

#include "SdScriptValueFunction.h"

class SdScriptValueFunModelPinTqfp : public SdScriptValueFunction
  {
    Sd3drModel *mModel;
  public:
    SdScriptValueFunModelPinTqfp( Sd3drModel *model ) :
      SdScriptValueFunction( SD_SCRIPT_TYPE_FACE_LIST, SD_SCRIPT_TYPE_FLOAT, SD_SCRIPT_TYPE_FLOAT, SD_SCRIPT_TYPE_FLOAT, SD_SCRIPT_TYPE_FLOAT, SD_SCRIPT_TYPE_FLOAT ),
      mModel(model)
      {
      }

    //!
    //! \brief toFaceList Convert object to list of face each of which is list of vertex index
    //! \return           List of face each of which is list of vertex index
    //!
    virtual Sd3drFaceList toFaceList() const { return mModel->faceListPinTqfp( mParamList[0]->toFloat(), mParamList[1]->toFloat(), mParamList[2]->toFloat(), mParamList[3]->toFloat(), mParamList[4]->toFloat() ); }

  };

#endif // SDSCRIPTVALUEFUNMODELPINTQFP_H
