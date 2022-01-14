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

  The function builds model of tqfp pin
*/
#ifndef SDSCRIPTVALUEFUNMODELPINTQFP_H
#define SDSCRIPTVALUEFUNMODELPINTQFP_H

#include "SdScriptValueFunction.h"

class SdScriptValueFunModelPinTqfp : public SdScriptValueFunction
  {
  public:
    SdScriptValueFunModelPinTqfp() : SdScriptValueFunction( SD_SCRIPT_TYPE_MODEL, SD_SCRIPT_TYPE_FLOAT, SD_SCRIPT_TYPE_FLOAT, SD_SCRIPT_TYPE_FLOAT, SD_SCRIPT_TYPE_FLOAT, SD_SCRIPT_TYPE_FLOAT, SD_SCRIPT_TYPE_COLOR )
      {
      }

    // SdM3dValue interface
  public:
    //!
    //! \brief toModel Convert object to 3d model
    //! \return        3d model
    //!
    virtual SdScriptVal3dModel toModel() const override { return sd3dModelPinTqfp( mParamList[0]->toFloat(), mParamList[1]->toFloat(), mParamList[2]->toFloat(), mParamList[3]->toFloat(), mParamList[4]->toFloat(), mParamList[5]->toColor() ); }

  };

#endif // SDSCRIPTVALUEFUNMODELPINTQFP_H
