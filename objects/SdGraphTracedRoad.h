/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Tracing road segment presented by end points, net name, stratum and road width
*/
#ifndef SDGRAPHTRACEDROAD_H
#define SDGRAPHTRACEDROAD_H

#include "SdGraphTraced.h"
#include "SdPropRoad.h"
#include "SdStratum.h"
#include "SdSegment.h"

#define SD_TYPE_GRAPH_TRACE_ROAD "Road"

enum SdFlyMode {
  flyP1P2,  //Both ends fly synchronously
  flyP1,    //Fly p1 point with arbitraty angle
  flyP2,    //Fly p2 point with arbitraty angle
  flyP1_45, //Fly p1 point with angle multiple of 45 degree
  flyP2_45  //Fly p2 point with angle multiple of 45 degree
  };

class SdGraphTracedRoad : public SdGraphTraced
  {
    SdPropRoad mProp;    //Road properties
                         // - road width
                         // - net name which contains this road
                         // - stratum of road
    SdSegment  mSegment; //Road segemnt (End points of road)

    //Not save and used when road edit
    SdFlyMode  mFly;
  public:
    SdGraphTracedRoad();
    SdGraphTracedRoad( const SdPropRoad &prp, SdPoint a, SdPoint b );

    // SdObject interface
  public:
    virtual QString getType() const override;
    virtual quint64 getClass() const override;
    virtual void    cloneFrom(const SdObject *src) override;
    virtual void    writeObject(QJsonObject &obj) const override;
    virtual void    readObject(SdObjectMap *map, const QJsonObject obj) override;

    // SdGraph interface
  public:
    virtual void    saveState(SdUndo *undo) override;
    virtual void    moveComplete(SdPoint grid, SdUndo *undo) override;
    virtual void    move(SdPoint offset) override;

    //Properties service [Изменение свойствами]
    //Set properties of this object from prop
    virtual void    setProp(SdPropSelected &prop) override;
    //Get (append) properties from this object to prop
    virtual void    getProp(SdPropSelected &prop) override;
    virtual void    selectByPoint(const SdPoint p, SdSelector *selector) override;
    virtual void    selectByRect(const SdRect &r, SdSelector *selector) override;
    virtual void    select(SdSelector *selector) override;
    virtual void    prepareMove(SdUndo *undo) override;
    virtual void    setLayerUsage() override;
    virtual bool    isVisible() override;
    virtual SdRect  getOverRect() const override;
    virtual int     behindCursor(SdPoint p) override;
    virtual bool    getInfo(SdPoint p, QString &info, bool extInfo) override;
    virtual bool snapPoint(SdSnapInfo *snap) override;

    // SdGraphTraced interface
  public:
    virtual bool       isPointOnNet(SdPoint p, SdStratum stratum, QString *wireName, int *destStratum) override;
    virtual void       accumNetSegments(SdPlateNetList &netList) const override;
    virtual void       drawStratum(SdContext *dcx, int stratum) override;
    virtual void       accumBarriers(SdBarrierList &dest, int stratum, SdRuleId toWhich, const SdRuleBlock &blk) const override;
    virtual bool       isMatchNetAndStratum( const QString netName, SdStratum stratum ) const override;
    //Stratum of object
    virtual SdStratum  stratum() const override { return mProp.mStratum; }

  private:
    //Return layer for road stratum
    SdLayer *getLayer() const;
  };

#endif // SDGRAPHTRACEDROAD_H
