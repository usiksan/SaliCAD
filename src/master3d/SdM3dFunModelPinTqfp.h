#ifndef SDM3DFUNMODELPINTQFP_H
#define SDM3DFUNMODELPINTQFP_H

#include "SdM3dFunction.h"

class SdM3dFunModelPinTqfp : public SdM3dFunction
  {
  public:
    SdM3dFunModelPinTqfp() : SdM3dFunction( SDM3D_TYPE_MODEL, SDM3D_TYPE_FLOAT, SDM3D_TYPE_FLOAT, SDM3D_TYPE_FLOAT, SDM3D_TYPE_FLOAT, SDM3D_TYPE_FLOAT, SDM3D_TYPE_COLOR )
      {
      }

    // SdM3dValue interface
  public:
    virtual SdM3dModel toModel() const override { return sd3dModelPinTqfp( mParamList[0]->toFloat(), mParamList[1]->toFloat(), mParamList[2]->toFloat(), mParamList[3]->toFloat(), mParamList[4]->toFloat(), mParamList[5]->toColor() ); }

  };

#endif // SDM3DFUNMODELPINTQFP_H
