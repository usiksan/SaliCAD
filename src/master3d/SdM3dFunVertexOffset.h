#ifndef SDM3DFUNVERTEXOFFSET_H
#define SDM3DFUNVERTEXOFFSET_H

#include "SdM3dFunction.h"

class SdM3dFunVertexOffset : public SdM3dFunction
  {
  public:
    SdM3dFunVertexOffset();

    // SdM3dValue interface
  public:
    virtual QVector3D toVertex() const override;
  };

#endif // SDM3DFUNVERTEXOFFSET_H
