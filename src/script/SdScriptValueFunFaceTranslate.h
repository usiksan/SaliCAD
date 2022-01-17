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

  The function translates face with offset vector
*/
#ifndef SDSCRIPTVALUEFUNFACETRANSLATE_H
#define SDSCRIPTVALUEFUNFACETRANSLATE_H

#include "SdScriptValueFunction.h"

class SdScriptValueFunFaceTranslate : public SdScriptValueFunction
  {
  public:
    SdScriptValueFunFaceTranslate() : SdScriptValueFunction( SD_SCRIPT_TYPE_FACE, SD_SCRIPT_TYPE_FACE, SD_SCRIPT_TYPE_VERTEX ) { }

    // SdM3dValue interface
  public:
    //!
    //! \brief toFace Convert object to 3d face which is bound region and face color
    //! \return       3d face which is bound region and face color
    //!
    virtual SdScriptVal3dFace toFace() const override { return mParamList[0]->toFace().translate( mParamList[1]->toVertex() ); }
  };

#endif // SDSCRIPTVALUEFUNFACETRANSLATE_H
