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

  This function build chip body with bevel
*/
#ifndef SDSCRIPTVALUEFUNLFACEBODYBEVELED_H
#define SDSCRIPTVALUEFUNLFACEBODYBEVELED_H

#include "SdScriptValueFunction.h"

class SdScriptValueFunLFaceBodyBeveled : public SdScriptValueFunction
  {
    Sd3drModel *mModel;
  public:
    SdScriptValueFunLFaceBodyBeveled( Sd3drModel *model ) :
      SdScriptValueFunction( SD_SCRIPT_TYPE_FACE_LIST, SD_SCRIPT_TYPE_FLOAT, SD_SCRIPT_TYPE_FLOAT, SD_SCRIPT_TYPE_FLOAT, SD_SCRIPT_TYPE_FLOAT, SD_SCRIPT_TYPE_FLOAT, SD_SCRIPT_TYPE_FLOAT ),
      mModel(model)
      {
      }


    //!
    //! \brief toFaceList Convert object to list of face each of which is list of vertex index
    //! \return           List of face each of which is list of vertex index
    //!
    virtual Sd3drFaceList        toFaceList() const override
      {
      return mModel->faceListBodyBeveled( mParamList[0]->toFloat(), mParamList[1]->toFloat(), mParamList[2]->toFloat(), mParamList[3]->toFloat(),
          mParamList[4]->toFloat(), mParamList[5]->toFloat() );
      }
  };

#endif // SDSCRIPTVALUEFUNLFACEBODYBEVELED_H
