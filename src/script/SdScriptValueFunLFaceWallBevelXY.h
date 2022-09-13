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

  The function builds wall from two faces
*/
#ifndef SDSCRIPTVALUEFUNLFACEWALLBEVELXY_H
#define SDSCRIPTVALUEFUNLFACEWALLBEVELXY_H

#include "SdScriptValueFunction.h"

class SdScriptValueFunLFaceWallBevelXY : public SdScriptValueFunction
  {
    Sd3drModel *mModel;
  public:
    SdScriptValueFunLFaceWallBevelXY( Sd3drModel *model ) :
      SdScriptValueFunction( SD_SCRIPT_TYPE_FACE_LIST, SD_SCRIPT_TYPE_FACE, SD_SCRIPT_TYPE_FACE, SD_SCRIPT_TYPE_FLOAT, SD_SCRIPT_TYPE_FLOAT ),
      mModel(model)
      {
      }

    //!
    //! \brief toFaceList Convert object to list of face each of which is list of vertex index
    //! \return           List of face each of which is list of vertex index
    //!
    virtual Sd3drFaceList  toFaceList() const override { return mModel->faceListWallBevelXY( mParamList[0]->toFace(), mParamList[1]->toFace(), mParamList[2]->toFloat(), mParamList[3]->toFloat() ); }

  };

#endif // SDSCRIPTVALUEFUNLFACEWALLBEVELXY_H
