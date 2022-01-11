#ifndef SDM3DVARIABLEGRAPH_H
#define SDM3DVARIABLEGRAPH_H

#include "SdM3dVariable.h"

class SdM3dVariableGraph : public SdM3dVariable
  {
    SdScriptVal2dGraph mValue;
  public:
    SdM3dVariableGraph() : mValue(QVector3D(), QVector3D()) {}



    // SdM3dValue interface
  public:
    virtual char       type() const override { return SD_SCRIPT_TYPE_GRAPH; }
    virtual SdScriptVal2dGraph toGraph() const override { return mValue; }

    // SdM3dVariable interface
  public:
    virtual void       assign(SdM3dValuePtr src) override { mValue = src->toGraph(); }
  };

#endif // SDM3DVARIABLEGRAPH_H
