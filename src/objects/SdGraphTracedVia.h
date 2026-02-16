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

    const SdPropVia  &propVia() const { return mProp; }
    // SdObject interface
  public:
    virtual QString   getType() const override;
    virtual SdClass   getClass() const override;
    //!
    //! \brief cloneFrom Overrided function. We copy object from source
    //! \param src       Source of object from which copy must be made
    //! \param copyMap   Structure for mapping copying substitutes
    //! \param next      Make simple or next copy. Next copy available not for all objects.
    //!                  For example: pin name A23 with next copy return A24
    //!
    virtual void      cloneFrom( const SdObject *src, SdCopyMap &copyMap, bool next ) override;

    //!
    //! \brief json Overloaded function to write object content into json writer
    //!             Overrided function
    //! \param js   Json writer
    //!
    virtual void      json( SdJsonWriter &js ) const override;

    //!
    //! \brief json Overloaded function to read object content from json reader
    //!             Overrided function
    //! \param js   Json reader
    //!
    virtual void      json( const SdJsonReader &js ) override;

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
    virtual SdPropStratum stratum() const override;
    virtual bool      isPointOnNet(SdPoint p, SdPropStratum stratum, QString *netName, int *destStratum) override;
    virtual void      accumNetSegments( SdPlateNetContainer *netContainer ) override;
    virtual void      drawStratum(SdContext *dcx, int stratum) override;
    virtual void      accumBarriers(SdBarrierList &dest, int stratum, SdRuleId toWhich, const SdRuleBlock &blk) const override;
    virtual bool      isMatchNetAndStratum(const QString netName, SdPropStratum stratum) const override;
    virtual void      accumWindows(SdPolyWindowList &dest, int stratum, int gap, const QString netName) const override;
    //Check if via linked to point
    virtual bool      isLinked( SdPoint a, SdPropStratum stratum, QString netName ) const override;
  };

#endif // SDGRAPHTRACEDVIA_H
