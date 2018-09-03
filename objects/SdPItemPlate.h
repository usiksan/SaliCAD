/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Graphical PCB layout with component placement
*/
#ifndef SDPITEMPLATE_H
#define SDPITEMPLATE_H

#include "SdProjectItem.h"
#include "SdRect.h"
#include "SdStratum.h"
#include "SdGraphTraced.h"
#include "SdPlateNet.h"
#include "SdPadAssociation.h"
#include "SdRuleBlock.h"
#include <QMap>
#include <QVector>

class SdGraphPartImp;
class SdGraphSymImp;
class SdPItemPart;
class SdPItemSymbol;
class SdPItemComponent;

#define SD_TYPE_PLATE "Plate"



class SdPItemPlate : public SdProjectItem
  {
    SdRect            mPartRow;           //Row for allocation autoinserted parts
    SdPadAssociation  mPadAssociation;    //Pad to pin association table
    SdRuleBlockList   mRules;             //Rules table
    QMap<QString,int> mRulesMap;          //Net, class names -to- rule map
    qint32            mStratumCount;      //Tracing stratum (layers) count

    //Not saved
    SdRatNet          mRatNet;            //Rat net is unconnected pairs
    bool              mRatNetDirty;
//    QString           mRuleCacheNetName;  //Net name cache for rule extract
//    int               mRuleCacheStratum;  //Stratum for rule extract
//    int               mRuleCacheBlockId;  //Block id for net name and stratum

    SdRectList        mRuleErrors;        //Indicator rule errors
  public:
    SdPItemPlate();

    //Stratum count
    int                    stratumCount() const { return mStratumCount; }
    void                   setStratumCount( int sc ) { mStratumCount = sc; mRatNetDirty = true; }

    SdGraphPartImp        *allocPartImp(int *section, SdPItemPart *part, SdPItemComponent *comp, SdPItemSymbol *sym, const SdStringMap &param, SdUndo *undo);

    //Pin-to-pad association
    //Full association table
    SdPadMap               getPadMap() const { return mPadAssociation.getMap(); }
    //Pad association table name
    QString                getPadAssociationName() const { return mPadAssociation.getName(); }
    //Draw pad
    void                   drawPad( SdContext *dc, SdPoint p, const QString pinType, int stratum ) const;
    //Setup new map and name
    void                   setPadAssociation(const QString nm, const SdPadMap &map, SdUndo *undo );
    //Return over pad polygon
    QPolygonF              getPadPolygon(SdPoint p, const QString pinType, int addon) const;
    //Append window for pin pad
    void                   appendPadWindow( SdPolyWindowList &dest, SdPoint p, const QString pinType, int gap, const QTransform &t );

    //Set flag to update rat net
    void                   setDirtyRatNet();

    //Draw rat net
    void                   drawRatNet( SdContext *dc );

    //Build rat net
    void                   buildRatNet();

    //Draw pcb for trace
    void                   drawTrace( SdContext *ctx, SdStratum curStratum, QString currentNetName );


    //Tracing rules
    //Rebuild rules map by scan all nets
    //void                   buildRulesMap();
    //Get rules for key
    const SdRuleBlock     &getRuleBlock( const QString key ) const;
    //Set rules for key
    void                   setRuleBlock( const QString key, const SdRuleBlock &src );
    //List of child rules
    QStringList            childRulesList( const QString key ) const;
    //Remove child rule
    void                   childRuleRemove( const QString key, const QString child );
    //Append child rule
    void                   childRuleAppend( const QString key, const QString child );
    //Get one rule for given net
    int                    ruleForNet(int stratum, const QString netName, SdRuleId ruleId);
    //Build rules block for given net
    void                   ruleBlockForNet(int stratum, const QString netName, SdRuleBlock &blockDest );


    //Tracing tests
    bool                   isAvailableSegment( SdPoint start, SdPoint &stop, int stratum, const QString netName, int width );

    //Accum bariers
    void                   accumBarriers( quint64 mask, SdBarrierList &dest, int stratum, SdRuleId toWhich, const SdRuleBlock &rule );

    //Check rules
    void                   checkRules(std::function<bool()> fun1);

    //Rule errors count
    int                    ruleErrorsCount() const { return mRuleErrors.count(); }

    //Rule error area
    SdRect                 ruleError( int i ) const { if( i < 0 || i >= mRuleErrors.count() ) return SdRect(); return mRuleErrors.at(i); }

    //Draw rule error indicators
    void                   drawRuleErrors( SdContext *dc );

    //Read-write rules
    QJsonObject            writeRuleMap() const;
    QJsonArray             writeRuleTable() const;
    void                   readRuleMap( const QJsonObject obj);
    void                   readRuleTable( const QJsonArray ar);


    //Renumeration implements
    void                   renumeration();

    // SdObject interface
  public:
    virtual QString        getType() const override;
    virtual quint64        getClass() const override;
    virtual void           cloneFrom(const SdObject *src) override;
    virtual void           writeObject(QJsonObject &obj) const override;
    virtual void           readObject(SdObjectMap *map, const QJsonObject obj) override;

    // SdProjectItem interface
  public:
    virtual QString        getIconName() const override;
    virtual quint64        getAcceptedObjectsMask() const override;

  private:
    void clearRuleCache();

    //Retrive rule starting with blockId
    int  ruleFromId(int stratumIndex, int blockId , int ruleId);

    //Check intersection on barriers and form errorList
    void checkIntersection(const SdBarrierList &src, const SdBarrierList &dst, bool excludeSameIndex );
  };

#endif // SDPITEMPLATE_H
