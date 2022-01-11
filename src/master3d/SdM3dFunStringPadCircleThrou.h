#ifndef SDM3DFUNSTRINGPADCIRCLETHROU_H
#define SDM3DFUNSTRINGPADCIRCLETHROU_H

#include "SdM3dFunction.h"

class SdM3dFunStringPadCircleThrou : public SdM3dFunction
  {
  public:
    //                                                  pad               pad diametr        hole diametr      mask addon
    SdM3dFunStringPadCircleThrou() : SdM3dFunction( SD_SCRIPT_TYPE_STRING, SD_SCRIPT_TYPE_FLOAT, SD_SCRIPT_TYPE_FLOAT, SD_SCRIPT_TYPE_FLOAT ) { }

    // SdM3dValue interface
  public:
    virtual QString toString() const override
      {
      return QString("c%1d%2m%3").arg( mParamList[0]->toFloat(), 0, 'f', 1 )
          .arg( mParamList[1]->toFloat(), 0, 'f', 1 )
          .arg( mParamList[2]->toFloat(), 0, 'f', 1 );
      }
  };

#endif // SDM3DFUNSTRINGPADCIRCLETHROU_H
