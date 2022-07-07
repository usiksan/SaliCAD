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
#ifndef SDSCRIPTVALUEFUNLFACEBOXMATRIX_H
#define SDSCRIPTVALUEFUNLFACEBOXMATRIX_H

#include "SdScriptValueFunction.h"

class SdScriptValueFunLFaceBoxMatrix : public SdScriptValueFunction
  {
    Sd3dModel *mModel;
  public:
    SdScriptValueFunLFaceBoxMatrix( Sd3dModel *model ) :
      SdScriptValueFunction( SD_SCRIPT_TYPE_FACE_LIST, SD_SCRIPT_TYPE_FLOAT, SD_SCRIPT_TYPE_FLOAT, SD_SCRIPT_TYPE_FLOAT, SD_SCRIPT_TYPE_MATRIX ),
      mModel(model)
      {
      }

    //!
    //! \brief toFaceList Convert object to list of face each of which is list of vertex index
    //! \return           List of face each of which is list of vertex index
    //!
    virtual Sd3drFaceList toFaceList() const override { return mModel->faceListBox( mParamList[0]->toFloat(), mParamList[1]->toFloat(), mParamList[2]->toFloat(), mParamList[3]->toMatrix() ); }

  };

#endif // SDSCRIPTVALUEFUNLFACEBOXMATRIX_H
