#ifndef SDM3DFUNBUILDVERTEX_H
#define SDM3DFUNBUILDVERTEX_H

#include "SdM3dFunction.h"

class SdM3dFunBuildVertex : public SdM3dFunction
  {
  public:
    SdM3dFunBuildVertex();

    // SdM3dValue interface
  public:
    virtual char      type() const override;
    virtual QVector3D toVertex() const override;

    // SdM3dFunction interface
  public:
    virtual int paramCount() const override;
    virtual char paramType(int index) const override;
  };

#endif // SDM3DFUNBUILDVERTEX_H
