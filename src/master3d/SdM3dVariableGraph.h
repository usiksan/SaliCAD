#ifndef SDM3DVARIABLEGRAPH_H
#define SDM3DVARIABLEGRAPH_H

#include "SdM3dVariable.h"

class SdM3dVariableGraph : public SdM3dVariable
  {
    SdM3dGraph mValue;
  public:
    SdM3dVariableGraph() : mValue() {}



    // SdM3dValue interface
  public:
    virtual SdM3dGraph toGraph() const override { return mValue; }

    // SdM3dVariable interface
  public:
    virtual void       assign(SdM3dValuePtr src) override { mValue = src->toGraph(); }
  };

#endif // SDM3DVARIABLEGRAPH_H
