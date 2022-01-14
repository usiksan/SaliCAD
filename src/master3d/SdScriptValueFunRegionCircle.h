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

  The function build circle region
*/
#ifndef SDSCRIPTVALUEFUNREGIONCIRCLE_H
#define SDSCRIPTVALUEFUNREGIONCIRCLE_H

#include "SdScriptValueFunction.h"

class SdScriptValueFunRegionCircle : public SdScriptValueFunction
  {
  public:
    SdScriptValueFunRegionCircle() : SdScriptValueFunction( SD_SCRIPT_TYPE_REGION, SD_SCRIPT_TYPE_FLOAT ) { }

    // SdM3dValue interface
  public:
    //!
    //! \brief toRegion Convert object to 3d region which is closed region of 3d vertex
    //! \return         3d region which is closed region of 3d vertex
    //!
    virtual SdScriptVal3dRegion toRegion() const override { return sd3dRegionCircle( mParamList[0]->toFloat() ); }

  };

#endif // SDSCRIPTVALUEFUNREGIONCIRCLE_H
