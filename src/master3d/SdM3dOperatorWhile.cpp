#include "SdM3dOperatorWhile.h"

SdM3dOperatorWhile::SdM3dOperatorWhile(SdScriptValue *cond, SdM3dOperator *block) :
  mCondition(cond),
  mBlock(block)
  {

  }

SdM3dOperatorWhile::~SdM3dOperatorWhile()
  {
  delete mCondition;
  delete mBlock;
  }


void SdM3dOperatorWhile::execute()
  {
  while( mCondition->toBool() )
    mBlock->execute();
  }
