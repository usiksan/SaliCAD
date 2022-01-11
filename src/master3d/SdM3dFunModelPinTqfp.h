#ifndef SDM3DFUNMODELPINTQFP_H
#define SDM3DFUNMODELPINTQFP_H

#include "SdM3dFunction.h"

class SdM3dFunModelPinTqfp : public SdM3dFunction
  {
  public:
    SdM3dFunModelPinTqfp() : SdM3dFunction( SD_SCRIPT_TYPE_MODEL, SD_SCRIPT_TYPE_FLOAT, SD_SCRIPT_TYPE_FLOAT, SD_SCRIPT_TYPE_FLOAT, SD_SCRIPT_TYPE_FLOAT, SD_SCRIPT_TYPE_FLOAT, SD_SCRIPT_TYPE_COLOR )
      {
      }

    // SdM3dValue interface
  public:
    virtual SdScriptVal3dModel toModel() const override { return sd3dModelPinTqfp( mParamList[0]->toFloat(), mParamList[1]->toFloat(), mParamList[2]->toFloat(), mParamList[3]->toFloat(), mParamList[4]->toFloat(), mParamList[5]->toColor() ); }

  };

#endif // SDM3DFUNMODELPINTQFP_H
