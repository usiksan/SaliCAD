/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  3d model programming language

  The function builds curved model from its bottom region and rotation params
*/
#ifndef SDM3DFUNMODELCURVE_H
#define SDM3DFUNMODELCURVE_H

#include "SdM3dFunction.h"

class SdM3dFunModelCurve : public SdM3dFunction
  {
  public:
    SdM3dFunModelCurve() :
      SdM3dFunction( SD_SCRIPT_TYPE_MODEL, SD_SCRIPT_TYPE_REGION, SD_SCRIPT_TYPE_VERTEX, SD_SCRIPT_TYPE_VERTEX, SD_SCRIPT_TYPE_FLOAT, SD_SCRIPT_TYPE_COLOR, SD_SCRIPT_TYPE_BOOL )
      {

      }

    // SdM3dValue interface
  public:
    virtual SdScriptVal3dModel toModel() const override
      {
      return sd3dModelCurve( mParamList[0]->toRegion(), mParamList[1]->toVertex(), mParamList[2]->toVertex(), mParamList[3]->toFloat(), mParamList[4]->toColor(), mParamList[5]->toBool() );
      }

  };

#endif // SDM3DFUNMODELCURVE_H
