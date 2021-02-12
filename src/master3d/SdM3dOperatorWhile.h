#ifndef SDM3DOPERATORWHILE_H
#define SDM3DOPERATORWHILE_H

#include "SdM3dOperator.h"
#include "SdM3dValue.h"

class SdM3dOperatorWhile : public SdM3dOperator
  {
    SdM3dValue    *mCondition;
    SdM3dOperator *mBlock;
  public:
    SdM3dOperatorWhile( SdM3dValue *cond, SdM3dOperator *block );
    ~SdM3dOperatorWhile();

    // SdM3dOperator interface
  public:
    virtual void execute() override;
  };

#endif // SDM3DOPERATORWHILE_H
