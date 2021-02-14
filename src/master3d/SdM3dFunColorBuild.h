#ifndef SDM3DFUNCOLORBUILD_H
#define SDM3DFUNCOLORBUILD_H

#include "SdM3dFunction.h"

class SdM3dFunColorBuild : public SdM3dFunction
  {
  public:
    SdM3dFunColorBuild();

    // SdM3dValue interface
  public:
    virtual QColor toColor() const override;

    static  QColor colorBuild( float r, float g, float b, float a );
  };


#endif // SDM3DFUNCOLORBUILD_H
