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
#ifndef SDRULEBLOCK_H
#define SDRULEBLOCK_H

#include <QList>
#include <QMap>
#include <QJsonObject>

//Available rules
//When change rules list You need change also
// in SdPItemPlate
//   Read-write rules
//   QJsonArray             writeRuleTable() const;
//   void                   readRuleTable( const QJsonArray ar);
// in SdEnvir - default list
#define ruleRoadWidth  0x0 //Road width
#define rulePadPad     0x1 //Gap between pads
#define ruleRoadRoad   0x2 //Gap between roads
#define ruleRoadPad    0x3 //Gap between pad and road
//#define ruleViaDiam    0x4 //Via
#define ruleLast       4

typedef int SdRuleId;

struct SdRuleBlock
  {
    qint32 mRules[ruleLast];

    SdRuleBlock();

    //Set all rules to value
    void setAllRule( int val );

    //Set rule block to value
    //void setRuleBlock( int val, SdRuleId first = ruleRoadWidth, SdRuleId last = ruleRoadRoad );

    //Set all rules to -1, i.e. inherited
    void clear();

    //Retrive individual rule
    int  rule( int ruleId, const SdRuleBlock &parent ) const;

    //Get actual rule block
    void getRuleBlock( SdRuleBlock &dest, const SdRuleBlock &parent ) const;

    //Write-read block
    QJsonObject write() const;
    void        read( const QJsonObject &obj );
  };

QDataStream &operator >> ( QDataStream &is, SdRuleBlock &blk );
QDataStream &operator << ( QDataStream &os, const SdRuleBlock &blk );

typedef QList<SdRuleBlock> SdRuleBlockList;

typedef QMap<QString,SdRuleBlock> SdRuleBlockMap;

#endif // SDRULEBLOCK_H
