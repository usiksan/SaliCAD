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
#ifndef SDSCRIPTVALUEFUNLFACESIMPLIFYXY_H
#define SDSCRIPTVALUEFUNLFACESIMPLIFYXY_H

#include "SdScriptValueFunction.h"

class SdScriptValueFunLFaceSimplifyXY : public SdScriptValueFunction
  {
    Sd3drModel *mModel;
  public:
    SdScriptValueFunLFaceSimplifyXY( Sd3drModel *model ) : SdScriptValueFunction( SD_SCRIPT_TYPE_FACE_LIST, SD_SCRIPT_TYPE_FACE ), mModel(model) { }

    // SdM3dValue interface
  public:
    //!
    //! \brief toFaceList Convert object to list of face each of which is list of vertex index
    //! \return           List of face each of which is list of vertex index
    //!
    virtual Sd3drFaceList toFaceList() const override { return mModel->faceListSimplifyXY( mParamList[0]->toFace() ); }
  };

#endif // SDSCRIPTVALUEFUNLFACESIMPLIFYXY_H
