#include "SdRuleBlock.h"

SdRuleBlock::SdRuleBlock() :
  mTopBlock(0)
  {
  clear();
  }




//Set all rules to value
void SdRuleBlock::setAllRule(int val)
  {
  for( int i = 0; i < ruleLast; i++ )
    mRules[i] = val;
  }



//Set rule block to value
void SdRuleBlock::setRuleBlock(int val, SdRuleId first, SdRuleId last)
  {
  for( int i = first; i <= last; i++ )
    mRules[i] = val;
  }




//Set all rules to -1, i.e. inherited
void SdRuleBlock::clear()
  {
  setAllRule(-1);
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
