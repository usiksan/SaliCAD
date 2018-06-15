#ifndef SDRULEBLOCK_H
#define SDRULEBLOCK_H

#include <QList>
#include <QDataStream>

//Available rules
//When change rules list You need change also
// in SdPItemPlate
//   Read-write rules
//   QJsonArray             writeRuleTable() const;
//   void                   readRuleTable( const QJsonArray ar);
// in SdEnvir - default list
enum SdRuleId {
  ruleWireWidth,
  rulePadPad,
  ruleWirePad,
  ruleWireWire,
  ruleLast
  };

#define RULE_BLOCK_PCB_ID     0
#define RULE_BLOCK_LAYER_ID   1
#define RULE_BLOCK_DEF_ID    31

#define RULE_BLOCK_PCB                   QStringLiteral("#@pcb")
#define RULE_BLOCK_LAYER(stratumIndex)   QStringLiteral("#@layer%1").arg(stratumIndex)
#define RULE_BLOCK_DEF                   QStringLiteral("#$def&")
#define RULE_BLOCK_NET(stratumIndex,net) (net + QString("#$stidx%1").arg(stratumIndex))


struct SdRuleBlock
  {
    qint32 mRules[ruleLast];
    qint32 mTopBlock;

    SdRuleBlock();

    //Set all rules to value
    void setAllRule( int val );

    //Set rule block to value
    void setRuleBlock( int val, SdRuleId first = ruleWireWidth, SdRuleId last = ruleWireWire );

    //Set all rules to -1, i.e. inherited
    void clear();

  };

QDataStream &operator >> ( QDataStream &is, SdRuleBlock &blk );
QDataStream &operator << ( QDataStream &os, const SdRuleBlock &blk );

typedef QList<SdRuleBlock> SdRuleBlockList;

#endif // SDRULEBLOCK_H
