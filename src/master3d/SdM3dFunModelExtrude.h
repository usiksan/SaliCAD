/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  3d model programming language

  The function extrudes the model from the region in the direction of the vector
*/

#ifndef SDM3DFUNMODELEXTRUDE_H
#define SDM3DFUNMODELEXTRUDE_H

#include "SdM3dFunction.h"

class SdM3dFunModelExtrude : public SdM3dFunction
  {
  public:
    SdM3dFunModelExtrude() :
      SdM3dFunction( SD_SCRIPT_TYPE_MODEL, SD_SCRIPT_TYPE_REGION, SD_SCRIPT_TYPE_FLOAT, SD_SCRIPT_TYPE_COLOR )
      {

      }

    // SdM3dValue interface
  public:
    virtual SdScriptVal3dModel toModel() const override
      {
      return sd3dModelExtrude( mParamList[0]->toRegion(), mParamList[1]->toFloat(), mParamList[2]->toColor() );
      }

  };

#endif // SDM3DFUNMODELEXTRUDE_H
