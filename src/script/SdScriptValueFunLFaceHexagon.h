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

  The function builds the hexagon model
*/
#ifndef SDSCRIPTVALUEFUNLFACEHEXAGON_H
#define SDSCRIPTVALUEFUNLFACEHEXAGON_H

#include "SdScriptValueFunction.h"

class SdScriptValueFunLFaceHexagon : public SdScriptValueFunction
  {
    Sd3drModel *mModel;
  public:
    SdScriptValueFunLFaceHexagon( Sd3drModel *model ) :
      //                                                  lenght                topLenght             height                width
      SdScriptValueFunction( SD_SCRIPT_TYPE_FACE_LIST, SD_SCRIPT_TYPE_FLOAT, SD_SCRIPT_TYPE_FLOAT, SD_SCRIPT_TYPE_FLOAT, SD_SCRIPT_TYPE_FLOAT ),
      mModel(model)
      {
      }

    //!
    //! \brief toFaceList Convert object to list of face each of which is list of vertex index
    //! \return           List of face each of which is list of vertex index
    //!
    virtual Sd3drFaceList toFaceList() const override { return mModel->faceListHexagon( mParamList[0]->toFloat(), mParamList[1]->toFloat(), mParamList[2]->toFloat(), mParamList[3]->toFloat() ); }

  };

#endif // SDSCRIPTVALUEFUNLFACEHEXAGON_H
