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

  The function translates the region with offset vector
*/
#ifndef SDSCRIPTVALUEFUNREGIONTRANSLATE_H
#define SDSCRIPTVALUEFUNREGIONTRANSLATE_H

#include "SdScriptValueFunction.h"

class SdScriptValueFunRegionTranslate : public SdScriptValueFunction
  {
  public:
    SdScriptValueFunRegionTranslate() : SdScriptValueFunction( SD_SCRIPT_TYPE_REGION, SD_SCRIPT_TYPE_REGION, SD_SCRIPT_TYPE_VERTEX ) { }

    // SdM3dValue interface
  public:
    //!
    //! \brief toRegion Convert object to 3d region which is closed region of 3d vertex
    //! \return         3d region which is closed region of 3d vertex
    //!
    virtual SdScriptVal3dRegion toRegion() const override { return sd3dRegionTranslate( mParamList[0]->toRegion(), mParamList[1]->toVertex() ); }

  };

#endif // SDSCRIPTVALUEFUNREGIONTRANSLATE_H
