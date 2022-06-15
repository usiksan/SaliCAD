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

  The function duplicate face with conversion matrix
*/
#ifndef SDSCRIPTVALUEFUNLFACEDUPLICATE_H
#define SDSCRIPTVALUEFUNLFACEDUPLICATE_H

#include "SdScriptValueFunction.h"

class SdScriptValueFunLFaceDuplicate : public SdScriptValueFunction
  {
    Sd3dModel *mModel;
  public:
    SdScriptValueFunLFaceDuplicate( Sd3dModel *model ) : SdScriptValueFunction( SD_SCRIPT_TYPE_FACE_LIST, SD_SCRIPT_TYPE_FACE_LIST, SD_SCRIPT_TYPE_MATRIX ), mModel(model) { }

    // SdM3dValue interface
  public:
    //!
    //! \brief toFaceList Convert object to list of face each of which is list of vertex index
    //! \return           List of face each of which is list of vertex index
    //!
    virtual Sd3drFaceList toFaceList() const override { return mModel->faceListDuplicate( mParamList[0]->toFaceList(), mParamList[1]->toMatrix() ); }
  };

#endif // SDSCRIPTVALUEFUNLFACEDUPLICATE_H
