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

  The function moves the region in the direction perpendicular to the plane of the region by the specified amount
*/
#ifndef SDSCRIPTVALUEFUNREGIONSHIFT_H
#define SDSCRIPTVALUEFUNREGIONSHIFT_H

#include "SdScriptValueFunction.h"

class SdScriptValueFunRegionShift : public SdScriptValueFunction
  {
  public:
    SdScriptValueFunRegionShift() : SdScriptValueFunction( SD_SCRIPT_TYPE_REGION, SD_SCRIPT_TYPE_REGION, SD_SCRIPT_TYPE_FLOAT ) { }

    // SdM3dValue interface
  public:
    //!
    //! \brief toRegion Convert object to 3d region which is closed region of 3d vertex
    //! \return         3d region which is closed region of 3d vertex
    //!
    virtual SdScriptVal3dRegion toRegion() const override { return sd3dRegionShift( mParamList[0]->toRegion(), mParamList[1]->toFloat() ); }

  };

#endif // SDSCRIPTVALUEFUNREGIONSHIFT_H
