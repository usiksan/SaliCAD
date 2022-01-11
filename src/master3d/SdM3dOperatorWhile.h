#ifndef SDM3DOPERATORWHILE_H
#define SDM3DOPERATORWHILE_H

#include "SdM3dOperator.h"
#include "SdScriptValue.h"

class SdM3dOperatorWhile : public SdM3dOperator
  {
    SdScriptValue    *mCondition;
    SdM3dOperator *mBlock;
  public:
    SdM3dOperatorWhile( SdScriptValue *cond, SdM3dOperator *block );
    ~SdM3dOperatorWhile();

    // SdM3dOperator interface
  public:
    virtual void execute() override;
  };

#endif // SDM3DOPERATORWHILE_H
