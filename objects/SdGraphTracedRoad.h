#ifndef SDGRAPHTRACEDROAD_H
#define SDGRAPHTRACEDROAD_H

#include "SdGraphTraced.h"
#include "SdPropRoad.h"
#include "SdStratum.h"

#define SD_TYPE_GRAPH_TRACE_ROAD "Road"

class SdGraphTracedRoad : public SdGraphTraced
  {
    QString    mNetName; //Net name which contains this road
    SdPropRoad mProp;    //Road properties
    SdStratum  mStratum; //Stratum of road
    SdPoint    mA,mB;    //End points of road

    //Not save and used when road edit
    bool       mFlyA;
    bool       mFlyB;
  public:
    SdGraphTracedRoad();

    // SdObject interface
  public:
    virtual QString getType() const override;
    virtual quint64 getClass() const override;
    virtual void    cloneFrom(const SdObject *src) override;
    virtual void    writeObject(QJsonObject &obj) const override;
    virtual void    readObject(SdObjectMap *map, const QJsonObject obj) override;

    // SdGraph interface
  public:
    virtual void saveState(SdUndo *undo) override;
    virtual void moveComplete(SdPoint grid, SdUndo *undo) override;
    virtual void move(SdPoint offset) override;
    virtual void setProp(SdPropSelected &prop) override;
    virtual void getProp(SdPropSelected &prop) override;
    virtual void selectByPoint(const SdPoint p, SdSelector *selector) override;
    virtual void selectByRect(const SdRect &r, SdSelector *selector) override;
    virtual void select(SdSelector *selector) override;
    virtual void prepareMove(SdUndo *undo) override;
    virtual void setLayerUsage() override;
    virtual bool isVisible() override;
    virtual SdRect getOverRect() const override;
    virtual int behindCursor(SdPoint p) override;
    virtual bool getInfo(SdPoint p, QString &info, bool extInfo) override;
    virtual bool snapPoint(SdSnapInfo *snap) override;

    // SdGraphTraced interface
  public:
    virtual bool isPointOnNet(SdPoint p, SdStratum stratum, QString &wireName) override;
    virtual void accumNetPoints(SdPlateNetList &netList) override;
    virtual void drawStratum(SdContext *dcx, int stratum) override;
    virtual void accumBarriers(SdBarrierList &dest, int stratum, SdRuleId toWhich, const SdRuleBlock &blk) const override;
  };

#endif // SDGRAPHTRACEDROAD_H
