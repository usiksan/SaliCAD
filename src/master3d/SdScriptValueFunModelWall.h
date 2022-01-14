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

  The function builds wall from bottom region and grow vector
*/
#ifndef SDSCRIPTVALUEFUNMODELWALL_H
#define SDSCRIPTVALUEFUNMODELWALL_H

#include "SdScriptValueFunction.h"

class SdScriptValueFunModelWall : public SdScriptValueFunction
  {
  public:
    SdScriptValueFunModelWall() :
      SdScriptValueFunction( SD_SCRIPT_TYPE_MODEL, SD_SCRIPT_TYPE_REGION, SD_SCRIPT_TYPE_VERTEX, SD_SCRIPT_TYPE_COLOR, SD_SCRIPT_TYPE_BOOL )
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
      return sd3dModelWall( mParamList[0]->toRegion(), mParamList[1]->toVertex(), mParamList[2]->toColor(), mParamList[3]->toBool() );
      }

  };

#endif // SDSCRIPTVALUEFUNMODELWALL_H
