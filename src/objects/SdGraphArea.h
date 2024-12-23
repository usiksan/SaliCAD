/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Separate pcb area of schematic sheet
*/
#ifndef SDGRAPHAREA_H
#define SDGRAPHAREA_H

#include "SdGraph.h"
#include "SdPointList.h"
#include "SdPropLine.h"
#include "SdPropText.h"
#include <QSet>

#define SD_TYPE_AREA "Area"

class SdPItemPlate;
class SdPItemSheet;

class SdGraphArea : public SdGraph
  {
    SdPointList   mRegion;     //Bound region of area
    SdPropLine    mRegionProp; //Region bounds line properties
    SdPItemPlate *mPlate;      //Plate area assotiated with
    QSet<int>     mFlyIndex;   //Fly vertex numbers
  public:
    SdGraphArea();
    SdGraphArea( const SdPointList list, const SdPropLine &propLine, SdPItemPlate *plate );

    SdPItemPlate *getPlate() const { return mPlate; }

    SdPItemSheet *getSheet() const;

    bool          isPointInside( SdPoint p ) const;

    // SdObject interface
  public:
    virtual QString getType() const override { return QStringLiteral(SD_TYPE_AREA); }
    virtual quint64 getClass() const override { return dctArea; }
    virtual void    attach(SdUndo *undo) override;
    virtual void    detach(SdUndo *undo) override;
    //!
    //! \brief cloneFrom Overrided function. We copy object from source
    //! \param src       Source of object from which copy must be made
    //! \param copyMap   Structure for mapping copying substitutes
    //! \param next      Make simple or next copy. Next copy available not for all objects.
    //!                  For example: pin name A23 with next copy return A24
    //!
    virtual void    cloneFrom( const SdObject *src, SdCopyMap &copyMap, bool next ) override;

    //!
    //! \brief json Overloaded function to write object content into json writer
    //!             Overrided function
    //! \param js   Json writer
    //!
    virtual void    json( SdJsonWriter &js ) const override;

    //!
    //! \brief json Overloaded function to read object content from json reader
    //!             Overrided function
    //! \param js   Json reader
    //!
    virtual void    json( const SdJsonReader &js ) override;

    // SdGraph interface
  public:
    virtual void    saveState(SdUndo *undo) override;
    virtual void    moveComplete( SdPoint grid, SdUndo *undo ) override;
    virtual void    move(SdPoint offset) override;
    virtual void    rotate(SdPoint center, SdPropAngle angle) override;
    virtual void    mirror(SdPoint a, SdPoint b) override;
    virtual void    setProp(SdPropSelected &prop) override;
    virtual void    getProp(SdPropSelected &prop) override;
    virtual void    selectByPoint(const SdPoint p, SdSelector *selector) override;
    virtual void    selectByRect(const SdRect &r, SdSelector *selector) override;
    virtual void    select(SdSelector *selector) override;
    virtual void    setLayerUsage() override;
    virtual bool    isVisible() override;
    virtual SdRect  getOverRect() const override;
    virtual void    draw(SdContext *dc) override;
    virtual int     behindCursor(SdPoint p) override;
    //Find snap point on object
    virtual void    snapPoint(SdSnapInfo *snap) override;
  };

#endif // SDGRAPHAREA_H
