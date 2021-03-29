#ifndef SDM3DFUNMODELHEXAGON_H
#define SDM3DFUNMODELHEXAGON_H

#include "SdM3dFunction.h"

class SdM3dFunModelHexagon : public SdM3dFunction
  {
  public:
    SdM3dFunModelHexagon() : SdM3dFunction( SDM3D_TYPE_MODEL, SDM3D_TYPE_FLOAT, SDM3D_TYPE_FLOAT, SDM3D_TYPE_FLOAT, SDM3D_TYPE_FLOAT, SDM3D_TYPE_COLOR )
      {
      }

    // SdM3dValue interface
  public:
    virtual SdM3dModel toModel() const override { return sd3dModelHexagon( mParamList[0]->toFloat(), mParamList[1]->toFloat(), mParamList[2]->toFloat(), mParamList[3]->toFloat(), mParamList[4]->toColor() ); }
  };

#endif // SDM3DFUNMODELHEXAGON_H
