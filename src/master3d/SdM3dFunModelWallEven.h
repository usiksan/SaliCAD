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
      SdM3dFunction( SDM3D_TYPE_MODEL, SDM3D_TYPE_REGION, SDM3D_TYPE_VERTEX, SDM3D_TYPE_COLOR )
      {

      }

    // SdM3dValue interface
  public:
    virtual SdM3dModel toModel() const override
      {
      return sd3dModelWallEven( mParamList[0]->toRegion(), mParamList[1]->toVertex(), mParamList[2]->toColor() );
      }

  };

#endif // SDM3DFUNMODELWALLEVEN_H
