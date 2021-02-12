#include "SdM3dOperatorBlock.h"

SdM3dOperatorBlock::SdM3dOperatorBlock()
  {

  }

SdM3dOperatorBlock::~SdM3dOperatorBlock()
  {
  qDeleteAll( mOperatorList );
  }


void SdM3dOperatorBlock::execute()
  {
  for( auto ptr : qAsConst(mOperatorList) )
    ptr->execute();
  }
