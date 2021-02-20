#ifndef SDM3DFUNVERTEXBUILD_H
#define SDM3DFUNVERTEXBUILD_H

#include "SdM3dFunction.h"

class SdM3dFunVertexBuild : public SdM3dFunction
  {
  public:
    SdM3dFunVertexBuild();

    // SdM3dValue interface
  public:
    virtual QVector3D toVertex() const override;
  };

#endif // SDM3DFUNVERTEXBUILD_H
