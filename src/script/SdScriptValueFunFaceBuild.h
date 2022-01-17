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
#ifndef SDSCRIPTVALUEFUNFACEBUILD_H
#define SDSCRIPTVALUEFUNFACEBUILD_H

#include "SdScriptValueFunction.h"

class SdScriptValueFunFaceBuild : public SdScriptValueFunction
  {
  public:
    SdScriptValueFunFaceBuild() : SdScriptValueFunction( SD_SCRIPT_TYPE_FACE, SD_SCRIPT_TYPE_REGION, SD_SCRIPT_TYPE_COLOR ) { }

    // SdM3dValue interface
  public:
    //!
    //! \brief toFace Convert object to 3d face which is bound region and face color
    //! \return       3d face which is bound region and face color
    //!
    virtual SdScriptVal3dFace toFace() const override { return SdScriptVal3dFace( mParamList[0]->toRegion(), mParamList[1]->toColor() ); }

  };


#endif // SDSCRIPTVALUEFUNFACEBUILD_H
