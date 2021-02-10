#ifndef SDM3DFUNBUILDBOX_H
#define SDM3DFUNBUILDBOX_H

#include "SdM3dFunction.h"

class SdM3dFunBuildBox : public SdM3dFunction
  {
  public:
    SdM3dFunBuildBox();

    // SdM3dValue interface
  public:
    virtual char type() const override;
    virtual SdM3dModel toModel() const override;

    // SdM3dFunction interface
  public:
    virtual int paramCount() const override;
    virtual char paramType(int index) const override;

    static SdM3dModel box(float w, float h, float z , QColor color);
  };

#endif // SDM3DFUNBUILDBOX_H
