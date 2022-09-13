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

  The function add extrudes the model from the top side of source model in the direction of the normal vector
  with shift amount
*/
#ifndef SDSCRIPTVALUEFUNLFACEHOLESXY_H
#define SDSCRIPTVALUEFUNLFACEHOLESXY_H

#include "SdScriptValueFunction.h"

class SdScriptValueFunLFaceHolesXY : public SdScriptValueFunction
  {
    Sd3drModel *mModel;
  public:
    //                                                                                                       src                  holeList
    SdScriptValueFunLFaceHolesXY( Sd3drModel *model ) : SdScriptValueFunction( SD_SCRIPT_TYPE_FACE_LIST, SD_SCRIPT_TYPE_FACE, SD_SCRIPT_TYPE_FACE_LIST ), mModel(model) { }

    // SdScriptValue interface
  public:
    //!
    //! \brief toFaceList Convert object to list of face each of which is list of vertex index
    //! \return           List of face each of which is list of vertex index
    //!
    virtual Sd3drFaceList toFaceList() const override { return mModel->faceListHolesXY( mParamList[0]->toFace(), mParamList[1]->toFaceList() ); }
  };

#endif // SDSCRIPTVALUEFUNLFACEHOLESXY_H
