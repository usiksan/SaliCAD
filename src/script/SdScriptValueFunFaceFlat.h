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

  The function builds face from face contour and face color
*/
#ifndef SDSCRIPTVALUEFUNFACEFLAT_H
#define SDSCRIPTVALUEFUNFACEFLAT_H

#include "SdScriptValueFunction.h"

class SdScriptValueFunFaceFlat : public SdScriptValueFunction
  {
    Sd3drModel *mModel;
  public:
    SdScriptValueFunFaceFlat( Sd3drModel *model ) : SdScriptValueFunction( SD_SCRIPT_TYPE_FACE, SD_SCRIPT_TYPE_VERTEX, SD_SCRIPT_TYPE_FLOAT, SD_SCRIPT_TYPE_AFLOAT ), mModel(model) { }

    // SdM3dValue interface
  public:
    //!
    //! \brief toFace Convert object to list of vertex index which is face region
    //! \return       List of vertex index which is face region
    //!
    virtual Sd3drFace toFace() const override { return mModel->faceFlat( mParamList[0]->toVertexIndex(), mParamList[2]->toFloatList(), mParamList[1]->toFloat() ); }

  };


#endif // SDSCRIPTVALUEFUNFACEFLAT_H
