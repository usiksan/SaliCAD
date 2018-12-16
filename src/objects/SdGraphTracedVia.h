#ifndef SDGRAPHTRACEDVIA_H
#define SDGRAPHTRACEDVIA_H

#include "SdGraphTraced.h"
#include "SdPropVia.h"

#define SD_TYPE_GRAPH_TRACE_VIA "Via"

class SdGraphTracedVia : public SdGraphTraced
  {
    SdPoint   mPosition; //Via position
    SdPropVia mProp;     //Via properties
  public:
    SdGraphTracedVia();
    SdGraphTracedVia( SdPoint pos, const SdPropVia &prp );
    //SdGraphTracedVia( const QString netName, SdStratum stratum, const QString padType, SdPoint pos );

    SdPoint           position() const { return mPosition; }
    // SdObject interface
  public:
    virtual QString   getType() const override;
    virtual SdClass   getClass() const override;
    virtual void      cloneFrom(const SdObject *src) override;
    virtual void      writeObject(QJsonObject &obj) const override;
    virtual void      readObject(SdObjectMap *map, const QJsonObject obj) override;

    // SdGraph interface
  public:
    virtual void      saveState(SdUndo *undo) override;
    virtual void      moveComplete(SdPoint grid, SdUndo *undo) override;
    virtual void      move(SdPoint offset) override;
    virtual void      setProp(SdPropSelected &prop) override;
    virtual void      getProp(SdPropSelected &prop) override;
    virtual void      selectByPoint(const SdPoint p, SdSelector *selector) override;
    virtual void      selectByRect(const SdRect &r, SdSelector *selector) override;
    virtual void      select(SdSelector *selector) override;
    virtual void      prepareMove(SdUndo *undo) override;
    virtual bool      isVisible() override;
    virtual SdRect    getOverRect() const override;
    virtual int       behindCursor(SdPoint p) override;
    virtual bool      getInfo(SdPoint p, QString &info, bool extInfo) override;
    //Find snap point on object
    virtual void      snapPoint(SdSnapInfo *snap) override;

    // SdGraphTraced interface
  public:
    virtual SdStratum stratum() const override;
    virtual bool      isPointOnNet(SdPoint p, SdStratum stratum, QString *netName, int *destStratum) override;
    virtual void      accumNetSegments(SdPlateNetList &netList) const override;
    virtual void      drawStratum(SdContext *dcx, int stratum) override;
    virtual void      accumBarriers(SdBarrierList &dest, int stratum, SdRuleId toWhich, const SdRuleBlock &blk) const override;
    virtual bool      isMatchNetAndStratum(const QString netName, SdStratum stratum) const override;
    virtual void      accumWindows(SdPolyWindowList &dest, int stratum, int gap, const QString netName) const override;
    virtual void      accumLinked( SdPoint a, SdStratum stratum, QString netName, SdSelector *sel ) override;
  };

#endif // SDGRAPHTRACEDVIA_H
