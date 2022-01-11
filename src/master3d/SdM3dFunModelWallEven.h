/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  3d model programming language

  The function builds wall from each pair of vertexes with grow vector and face color
*/
#ifndef SDM3DFUNMODELWALLEVEN_H
#define SDM3DFUNMODELWALLEVEN_H

#include "SdM3dFunction.h"

class SdM3dFunModelWallEven : public SdM3dFunction
  {
  public:
    SdM3dFunModelWallEven() :
      SdM3dFunction( SD_SCRIPT_TYPE_MODEL, SD_SCRIPT_TYPE_REGION, SD_SCRIPT_TYPE_VERTEX, SD_SCRIPT_TYPE_COLOR )
      {

      }

    // SdM3dValue interface
  public:
    virtual SdScriptVal3dModel toModel() const override
      {
      return sd3dModelWallEven( mParamList[0]->toRegion(), mParamList[1]->toVertex(), mParamList[2]->toColor() );
      }

  };

#endif // SDM3DFUNMODELWALLEVEN_H
