#include "SdM3dOperatorIf.h"

SdM3dOperatorIf::SdM3dOperatorIf( SdScriptValue *cond, SdM3dOperator *opTrue, SdM3dOperator *opFalse ) :
  mCondition(cond),
  mTrue(opTrue),
  mFalse(opFalse)
  {

  }


SdM3dOperatorIf::~SdM3dOperatorIf()
  {
  delete mCondition;
  delete mTrue;
  if( mFalse != nullptr )
    delete mFalse;
  }

void SdM3dOperatorIf::execute()
  {
  if( mCondition->toBool() )
    mTrue->execute();
  else if( mFalse != nullptr )
    mFalse->execute();
  }
