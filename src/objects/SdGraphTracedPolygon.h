/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Graphical traced polygon object. Traced object is object which represents printed polygin with windows.
*/
#ifndef SDGRAPHTRACEDPOLYGON_H
#define SDGRAPHTRACEDPOLYGON_H

#include "SdGraphTraced.h"
#include "SdPointList.h"
#include "SdPolyWindowList.h"

#define SD_TYPE_GRAPH_TRACE_POLYGON "TPoly"


class SdGraphTracedPolygon : public SdGraphTraced
  {
    SdPropPolygon    mProp;     //Polygon properties
                                // - gap for windows creation
                                // - net name which contains this polygon
                                // - stratum of polygon
    SdPointList      mRegion;   //Contour of polygon
    SdPolyWindowList mWindows;  //Windows of polygon

    QSet<int>        mFlyIndex; //Fly vertex numbers
  public:
    SdGraphTracedPolygon();
    SdGraphTracedPolygon( const SdPropPolygon &prp, SdPointList lst, SdPolyWindowList win );

    // SdObject interface
  public:
    virtual QString   getType() const override;
    virtual SdClass   getClass() const override;
    virtual void      attach(SdUndo *undo) override;
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
    virtual void      rotate(SdPoint center, SdPropAngle angle) override;
    virtual void      setProp(SdPropSelected &prop) override;
    virtual void      getProp(SdPropSelected &prop) override;
    virtual void      selectByPoint(const SdPoint p, SdSelector *selector) override;
    virtual void      selectByRect(const SdRect &r, SdSelector *selector) override;
    virtual void      select(SdSelector *selector) override;
    virtual void      setLayerUsage() override;
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
    virtual void      accumNetSegments( SdPlateNetContainer *netContainer ) override;
    virtual void      drawStratum(SdContext *dcx, int stratum) override;
    virtual void accumBarriers(SdBarrierList &dest, int stratum, SdRuleId toWhich, const SdRuleBlock &blk) const override;
    virtual bool      isMatchNetAndStratum(const QString netName, SdStratum stratum) const override;
    virtual void      accumWindows(SdPolyWindowList &dest, int stratum, int gap, const QString netName ) const override;
    //Check if polygon linked to point
    virtual bool      isLinked( SdPoint a, SdStratum stratum, QString netName ) const override;

  private:
    //Return layer for polygon
    SdLayer *getLayer() const;

    //Rebuild windows list
    void     rebuildWindows();

    //Return true if point is on polygon
    bool     isContains( SdPoint p ) const;
  };

#endif // SDGRAPHTRACEDPOLYGON_H
