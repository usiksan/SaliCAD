#ifndef SDM3DFUNVERTEXCENTEROFREGION_H
#define SDM3DFUNVERTEXCENTEROFREGION_H

#include "SdM3dFunction.h"

class SdM3dFunVertexCenterOfRegion : public SdM3dFunction
  {
  public:
    SdM3dFunVertexCenterOfRegion();

    // SdM3dValue interface
  public:
    virtual QVector3D toVertex() const override;

    static  QVector3D vertexCenterOfRegion( SdM3dRegion region );
  };

#endif // SDM3DFUNVERTEXCENTEROFREGION_H
