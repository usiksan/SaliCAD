#include "SdRuleBlock.h"

SdRuleBlock::SdRuleBlock() :
  mTopBlock(0)
  {
  clear();
  }




void SdRuleBlock::clear()
  {
  for( int i = 0; i < ruleLast; i++ )
    mRules[i] = -1;
  }



QDataStream &operator >>(QDataStream &is, SdRuleBlock &blk)
  {
  for( int i = 0; i < ruleLast; i++ )
    is >> blk.mRules[i];
  is >> blk.mTopBlock;
  return is;
  }



QDataStream &operator <<(QDataStream &os, const SdRuleBlock &blk)
  {
  for( int i = 0; i < ruleLast; i++ )
    os << blk.mRules[i];
  os << blk.mTopBlock;
  return os;
  }
