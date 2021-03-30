#ifndef SDM3DFUNMODELBODYBEVELED_H
#define SDM3DFUNMODELBODYBEVELED_H

#include "SdM3dFunction.h"

class SdM3dFunModelBodyBeveled : public SdM3dFunction
  {
  public:
    SdM3dFunModelBodyBeveled() : SdM3dFunction( SDM3D_TYPE_MODEL, SDM3D_TYPE_FLOAT, SDM3D_TYPE_FLOAT, SDM3D_TYPE_FLOAT, SDM3D_TYPE_FLOAT, SDM3D_TYPE_FLOAT, SDM3D_TYPE_FLOAT, SDM3D_TYPE_COLOR )
      {
      }


    // SdM3dValue interface
  public:
    virtual SdM3dModel toModel() const override
      {
      return sd3dModelBodyBeveled( mParamList[0]->toFloat(), mParamList[1]->toFloat(), mParamList[2]->toFloat(), mParamList[3]->toFloat(),
          mParamList[4]->toFloat(), mParamList[5]->toFloat(), mParamList[6]->toColor() );
      }
  };

#endif // SDM3DFUNMODELBODYBEVELED_H
