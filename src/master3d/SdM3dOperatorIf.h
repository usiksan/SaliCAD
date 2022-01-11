#ifndef SDM3DOPERATORIF_H
#define SDM3DOPERATORIF_H

#include "SdM3dOperator.h"
#include "SdScriptValue.h"

class SdM3dOperatorIf : public SdM3dOperator
  {
    SdScriptValue    *mCondition;
    SdM3dOperator *mTrue;
    SdM3dOperator *mFalse;
  public:
    SdM3dOperatorIf( SdScriptValue *cond, SdM3dOperator *opTrue, SdM3dOperator *opFalse = nullptr );

    ~SdM3dOperatorIf();

    // SdM3dOperator interface
  public:
    virtual void execute() override;
  };

#endif // SDM3DOPERATORIF_H
