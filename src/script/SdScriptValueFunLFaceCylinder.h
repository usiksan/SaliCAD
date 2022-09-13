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

  The function builds cylinder model from its size
*/
#ifndef SDSCRIPTVALUEFUNLFACECYLINDER_H
#define SDSCRIPTVALUEFUNLFACECYLINDER_H

#include "SdScriptValueFunction.h"

class SdScriptValueFunLFaceCylinder : public SdScriptValueFunction
  {
    Sd3drModel *mModel;
  public:
    SdScriptValueFunLFaceCylinder( Sd3drModel *model ) :
      SdScriptValueFunction( SD_SCRIPT_TYPE_FACE_LIST, SD_SCRIPT_TYPE_FLOAT, SD_SCRIPT_TYPE_FLOAT ),
      mModel(model)
      {

      }

    //!
    //! \brief toFaceList Convert object to list of face each of which is list of vertex index
    //! \return           List of face each of which is list of vertex index
    //!
    virtual Sd3drFaceList toFaceList() const override { return mModel->faceListCylinder( mParamList[0]->toFloat(), mParamList[1]->toFloat(), QMatrix4x4() ); }

  };

#endif // SDSCRIPTVALUEFUNLFACECYLINDER_H
