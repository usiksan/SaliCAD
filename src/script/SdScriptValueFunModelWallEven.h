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

  The function builds wall from each pair of vertexes with grow vector and face color
*/
#ifndef SDSCRIPTVALUEFUNMODELWALLEVEN_H
#define SDSCRIPTVALUEFUNMODELWALLEVEN_H

#include "SdScriptValueFunction.h"

class SdScriptValueFunModelWallEven : public SdScriptValueFunction
  {
  public:
    SdScriptValueFunModelWallEven() :
      SdScriptValueFunction( SD_SCRIPT_TYPE_MODEL, SD_SCRIPT_TYPE_REGION, SD_SCRIPT_TYPE_VERTEX, SD_SCRIPT_TYPE_COLOR )
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
      return sd3dModelWallEven( mParamList[0]->toRegion(), mParamList[1]->toVertex(), mParamList[2]->toColor() );
      }

  };

#endif // SDSCRIPTVALUEFUNMODELWALLEVEN_H
