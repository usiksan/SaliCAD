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

  The function builds curved model from its bottom region and rotation params
*/
#ifndef SDSCRIPTVALUEFUNMODELCURVE_H
#define SDSCRIPTVALUEFUNMODELCURVE_H

#include "SdScriptValueFunction.h"

class SdScriptValueFunModelCurve : public SdScriptValueFunction
  {
  public:
    SdScriptValueFunModelCurve() :
      SdScriptValueFunction( SD_SCRIPT_TYPE_MODEL, SD_SCRIPT_TYPE_REGION, SD_SCRIPT_TYPE_VERTEX, SD_SCRIPT_TYPE_VERTEX, SD_SCRIPT_TYPE_FLOAT, SD_SCRIPT_TYPE_COLOR, SD_SCRIPT_TYPE_BOOL )
      {

      }

    // SdM3dValue interface
  public:
    //!
    //! \brief toModel Convert object to 3d model
    //! \return        3d model
    //!
    virtual SdScriptVal3dModel toModel() const override
      {
      return sd3dModelCurve( mParamList[0]->toRegion(), mParamList[1]->toVertex(), mParamList[2]->toVertex(), mParamList[3]->toFloat(), mParamList[4]->toColor(), mParamList[5]->toBool() );
      }

  };

#endif // SDSCRIPTVALUEFUNMODELCURVE_H
