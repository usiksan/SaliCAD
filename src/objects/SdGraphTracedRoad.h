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
#include "SdPropStratum.h"
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

    //Return current segment of road
    const SdSegment   &segment() const { return mSegment; }

    //Return road properties
    const SdPropRoad  &propRoad() const { return mProp; }

    //Set new points of segment
    void               setSegment( SdPoint a, SdPoint b, SdUndo *undo );

    //Split road on two roads with p as division point
    void               splitRoad( SdPoint p, SdUndo *undo );

    //Union consequent segments
    void               utilize( SdUndo *undo );

    bool               isLinkedP1( SdPoint a, SdPropStratum stratum, QString netName ) const;
    bool               isLinkedP2( SdPoint a, SdPropStratum stratum, QString netName ) const;

    // SdObject interface
  public:
    virtual QString    getType() const override;
    virtual quint64    getClass() const override;
    //!
    //! \brief cloneFrom Overrided function. We copy object from source
    //! \param src       Source of object from which copy must be made
    //! \param copyMap   Structure for mapping copying substitutes
    //! \param next      Make simple or next copy. Next copy available not for all objects.
    //!                  For example: pin name A23 with next copy return A24
    //!
    virtual void       cloneFrom( const SdObject *src, SdCopyMap &copyMap, bool next ) override;

    //!
    //! \brief json Overloaded function to write object content into json writer
    //!             Overrided function
    //! \param js   Json writer
    //!
    virtual void       json( SdJsonWriter &js ) const override;

    //!
    //! \brief json Overloaded function to read object content from json reader
    //!             Overrided function
    //! \param js   Json reader
    //!
    virtual void       json( const SdJsonReader &js ) override;

    // SdGraph interface
  public:
    virtual void       saveState(SdUndo *undo) override;
    virtual void       moveComplete(SdPoint grid, SdUndo *undo) override;
    virtual void       move(SdPoint offset) override;

    //Properties service [Изменение свойствами]
    //Set properties of this object from prop
    virtual void       setProp(SdPropSelected &prop) override;
    //Get (append) properties from this object to prop
    virtual void       getProp(SdPropSelected &prop) override;
    virtual void       selectByPoint(const SdPoint p, SdSelector *selector) override;
    virtual void       selectByRect(const SdRect &r, SdSelector *selector) override;
    virtual void       select(SdSelector *selector) override;
    virtual void       prepareMove(SdUndo *undo) override;
    virtual void       setLayerUsage() override;
    virtual bool       isVisible() override;
    virtual SdRect     getOverRect() const override;
    virtual int        behindCursor(SdPoint p) override;
    virtual bool       getInfo(SdPoint p, QString &info, bool extInfo) override;
    //Find snap point on object
    virtual void       snapPoint(SdSnapInfo *snap) override;

    // SdGraphTraced interface
  public:
    virtual bool       isPointOnNet(SdPoint p, SdPropStratum stratum, QString *netName, int *destStratum) override;
    virtual void       accumNetSegments( SdPlateNetContainer *netContainer ) override;
    virtual void       drawStratum(SdContext *dcx, int stratum) override;
    virtual void       accumBarriers(SdBarrierList &dest, int stratum, SdRuleId toWhich, const SdRuleBlock &blk) const override;
    virtual bool       isMatchNetAndStratum( const QString netName, SdPropStratum stratum ) const override;
    virtual void       accumWindows(SdPolyWindowList &dest, int stratum, int gap, const QString netName ) const override;
    //Check if road linked to point
    virtual bool       isLinked( SdPoint a, SdPropStratum stratum, QString netName ) const override;
    //Stratum of object
    virtual SdPropStratum  stratum() const override { return mProp.mStratum; }

  private:
    //Return layer for road stratum
    SdLayer           *getLayer() const;

    //Single road linked to point. If more than one road then no roads return
    SdGraphTracedRoad *linkedRoad( SdPoint p );

    //Utilize on end p
    void               utilizeAtEnd( SdPoint p, SdUndo *undo );

    //Utilize over segment (i.e. if one segment is partial or full over another segment)
    void               utilizeOver( SdUndo *undo );
  };

typedef SdGraphTracedRoad *SdGraphTracedRoadPtr;

#endif // SDGRAPHTRACEDROAD_H
