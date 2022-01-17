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

  The function builds box model from its size
*/
#ifndef SDSCRIPTVALUEFUNMODELBOX_H
#define SDSCRIPTVALUEFUNMODELBOX_H

#include "SdScriptValueFunction.h"

class SdScriptValueFunModelBox : public SdScriptValueFunction
  {
  public:
    SdScriptValueFunModelBox() :
      SdScriptValueFunction( SD_SCRIPT_TYPE_MODEL, SD_SCRIPT_TYPE_FLOAT, SD_SCRIPT_TYPE_FLOAT, SD_SCRIPT_TYPE_FLOAT, SD_SCRIPT_TYPE_COLOR )
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
      return sd3dModelBox( mParamList[0]->toFloat(), mParamList[1]->toFloat(), mParamList[2]->toFloat(), mParamList[3]->toColor() );
      }
  };

#endif // SDSCRIPTVALUEFUNMODELBOX_H
