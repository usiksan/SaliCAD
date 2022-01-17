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

  The function builds cylinder model from its size
*/
#ifndef SDSCRIPTVALUEFUNMODELCYLINDER_H
#define SDSCRIPTVALUEFUNMODELCYLINDER_H

#include "SdScriptValueFunction.h"

class SdScriptValueFunModelCylinder : public SdScriptValueFunction
  {
  public:
    SdScriptValueFunModelCylinder() :
      SdScriptValueFunction( SD_SCRIPT_TYPE_MODEL, SD_SCRIPT_TYPE_FLOAT, SD_SCRIPT_TYPE_FLOAT, SD_SCRIPT_TYPE_COLOR )
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
      return sd3dModelCylinder( mParamList[0]->toFloat(), mParamList[1]->toFloat(), mParamList[2]->toColor() );
      }

  };

#endif // SDSCRIPTVALUEFUNMODELCYLINDER_H
