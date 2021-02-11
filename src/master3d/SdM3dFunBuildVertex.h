#ifndef SDM3DFUNBUILDVERTEX_H
#define SDM3DFUNBUILDVERTEX_H

#include "SdM3dFunction.h"

class SdM3dFunBuildVertex : public SdM3dFunction
  {
  public:
    SdM3dFunBuildVertex();

    // SdM3dValue interface
  public:
    virtual QVector3D toVertex() const override;
  };

#endif // SDM3DFUNBUILDVERTEX_H
