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

  The function builds duplicate of source face scaled and translated on appropriated params
*/
#ifndef SDSCRIPTVALUEFUNFACEDUPLICATESCALE_H
#define SDSCRIPTVALUEFUNFACEDUPLICATESCALE_H

#include "SdScriptValueFunction.h"

class SdScriptValueFunFaceDuplicateScale : public SdScriptValueFunction
  {
    Sd3drModel *mModel;
  public:
    //                                                                                                         source            scalex                 scaley                shift
    SdScriptValueFunFaceDuplicateScale( Sd3drModel *model ) : SdScriptValueFunction( SD_SCRIPT_TYPE_FACE, SD_SCRIPT_TYPE_FACE, SD_SCRIPT_TYPE_FLOAT, SD_SCRIPT_TYPE_FLOAT, SD_SCRIPT_TYPE_FLOAT ), mModel(model) { }

    // SdM3dValue interface
  public:
    //!
    //! \brief toFace Convert object to list of vertex index which is face region
    //! \return       List of vertex index which is face region
    //!
    virtual Sd3drFace toFace() const override { return mModel->faceDuplicateScale( mParamList[0]->toFace(), mParamList[1]->toFloat(), mParamList[2]->toFloat(), mParamList[3]->toFloat() ); }

  };

#endif // SDSCRIPTVALUEFUNFACEDUPLICATESCALE_H
