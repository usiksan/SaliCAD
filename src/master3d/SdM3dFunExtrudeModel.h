#ifndef SDM3DFUNEXTRUDEMODEL_H
#define SDM3DFUNEXTRUDEMODEL_H

#include "SdM3dFunction.h"

class SdM3dFunExtrudeModel : public SdM3dFunction
  {
  public:
    SdM3dFunExtrudeModel();

    // SdM3dValue interface
  public:
    virtual char       type() const override;
    virtual SdM3dModel toModel() const override;

    // SdM3dFunction interface
  public:
    virtual int  paramCount() const override;
    virtual char paramType(int index) const override;

    static SdM3dModel model(SdM3dRegion region, QVector3D vector, QColor color);
  };

#endif // SDM3DFUNEXTRUDEMODEL_H
