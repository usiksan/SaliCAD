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

  The function extrudes the model from the region in the direction of the vector
*/

#ifndef SDSCRIPTVALUEFUNMODELEXTRUDE_H
#define SDSCRIPTVALUEFUNMODELEXTRUDE_H

#include "SdScriptValueFunction.h"

class SdM3dFunModelExtrude : public SdScriptValueFunction
  {
  public:
    SdM3dFunModelExtrude() :
      SdScriptValueFunction( SD_SCRIPT_TYPE_MODEL, SD_SCRIPT_TYPE_REGION, SD_SCRIPT_TYPE_FLOAT, SD_SCRIPT_TYPE_COLOR )
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
      return sd3dModelExtrude( mParamList[0]->toRegion(), mParamList[1]->toFloat(), mParamList[2]->toColor() );
      }

  };

#endif // SDSCRIPTVALUEFUNMODELEXTRUDE_H
