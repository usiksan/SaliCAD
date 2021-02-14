#ifndef SDM3DPARTMODEL_H
#define SDM3DPARTMODEL_H

#include "SdM3dVariable.h"

class SdPItemPart;

class SdM3dPartModel : public SdM3dVariable
  {
    SdPItemPart *mPart;
  public:
    SdM3dPartModel( SdPItemPart *part );

    // SdM3dValue interface
  public:
    virtual char type() const override { return SDM3D_TYPE_MODEL; }

    // SdM3dVariable interface
  public:
    virtual void assign(SdM3dValuePtr src) override;
  };

#endif // SDM3DPARTMODEL_H
