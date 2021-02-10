#ifndef SDM3DOPERATORASSIGN_H
#define SDM3DOPERATORASSIGN_H

#include "SdM3dOperator.h"
#include "SdM3dVariable.h"

class SdM3dOperatorAssign : public SdM3dOperator
  {
    SdM3dVariable *mVariable;
    SdM3dValue    *mValue;
  public:
    SdM3dOperatorAssign( SdM3dVariable *var, SdM3dValue *val ) : mVariable(var), mValue(val) {}
    ~SdM3dOperatorAssign() { delete mValue; }

    // SdM3dOperator interface
  public:
    virtual void execute() override { mVariable->assign( mValue ); }
  };

#endif // SDM3DOPERATORASSIGN_H
