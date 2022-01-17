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

  This function build chip body with bevel
*/
#ifndef SDSCRIPTVALUEFUNMODELBODYBEVELED_H
#define SDSCRIPTVALUEFUNMODELBODYBEVELED_H

#include "SdScriptValueFunction.h"

class SdScriptValueFunModelBodyBeveled : public SdScriptValueFunction
  {
  public:
    SdScriptValueFunModelBodyBeveled() : SdScriptValueFunction( SD_SCRIPT_TYPE_MODEL, SD_SCRIPT_TYPE_FLOAT, SD_SCRIPT_TYPE_FLOAT, SD_SCRIPT_TYPE_FLOAT, SD_SCRIPT_TYPE_FLOAT, SD_SCRIPT_TYPE_FLOAT, SD_SCRIPT_TYPE_FLOAT, SD_SCRIPT_TYPE_COLOR )
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
      return sd3dModelBodyBeveled( mParamList[0]->toFloat(), mParamList[1]->toFloat(), mParamList[2]->toFloat(), mParamList[3]->toFloat(),
          mParamList[4]->toFloat(), mParamList[5]->toFloat(), mParamList[6]->toColor() );
      }
  };

#endif // SDSCRIPTVALUEFUNMODELBODYBEVELED_H
