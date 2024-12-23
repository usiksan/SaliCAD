/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  All trace rules contained in block of rules. One block describes all rules for
  current tracing.

  The rule blocks are grouped into a tree. The leaves of the tree are responsible for the assigned rules.
  If no rule is assigned in the block, tree climbing is performed to find the rules.
  The rules of the root block is always assigned. Not only one rule can be unassigned,
  but the whole block. In this case, the lifting is also performed on the tree.

  [Блоки правил сгруппированы в дерево. Листья дерева отвечают за назначенные правила.
  Если правило в блоке не назначено, то для поиска правил выполняется подъем по дереву.
  Правила корневого блока всегда назначены. Может быть не назначено не только одно правило,
  но и целый блок. В этом случае также выполняется подъем по дереву.

  Вверху расположены правила платы.
  Далее тридцать блоков правил для слоев.]

*/

#include "SdRuleBlock.h"

SdRuleBlock::SdRuleBlock()
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
//void SdRuleBlock::setRuleBlock(int val, SdRuleId first, SdRuleId last)
//  {
//  for( int i = first; i <= last; i++ )
//    mRules[i] = val;
//  }




//Set all rules to -1, i.e. inherited
void SdRuleBlock::clear()
  {
  setAllRule(-1);
  }




//Retrive individual rule
int SdRuleBlock::rule(int ruleId, const SdRuleBlock &parent) const
  {
  Q_ASSERT( ruleId >= 0 && ruleId < ruleLast );
  if( mRules[ruleId] >= 0 )
    return mRules[ruleId];
  return parent.mRules[ruleId];
  }




//Get actual rule block
void SdRuleBlock::getRuleBlock(SdRuleBlock &dest, const SdRuleBlock &parent) const
  {
  for( int i = 0; i < ruleLast; i++ )
    dest.mRules[i] = mRules[i] >= 0 ? mRules[i] : parent.mRules[i];
  }



//!
//! \brief json Overloaded function to write object content into json writer
//! \param js   Json writer
//!
void SdRuleBlock::json(SvJsonWriter &js) const
  {
  js.jsonInt( QStringLiteral("Width"), mRules[ruleRoadWidth] );
  js.jsonInt( QStringLiteral("PadPad"), mRules[rulePadPad] );
  js.jsonInt( QStringLiteral("RoadPad"), mRules[ruleRoadPad] );
  js.jsonInt( QStringLiteral("RoadRoad"), mRules[ruleRoadRoad] );
  }




//!
//! \brief json Overloaded function to read object content from json reader
//! \param js   Json reader
//!
void SdRuleBlock::json(const SvJsonReader &js)
  {
  js.jsonInt( QStringLiteral("Width"), mRules[ruleRoadWidth] );
  js.jsonInt( QStringLiteral("PadPad"), mRules[rulePadPad] );
  js.jsonInt( QStringLiteral("RoadPad"), mRules[ruleRoadPad] );
  js.jsonInt( QStringLiteral("RoadRoad"), mRules[ruleRoadRoad] );
  }









QDataStream &operator >>(QDataStream &is, SdRuleBlock &blk)
  {
  for( int i = 0; i < ruleLast; i++ )
    is >> blk.mRules[i];
  //is >> blk.mTopBlock;
  return is;
  }



QDataStream &operator <<(QDataStream &os, const SdRuleBlock &blk)
  {
  for( int i = 0; i < ruleLast; i++ )
    os << blk.mRules[i];
  //os << blk.mTopBlock;
  return os;
  }
