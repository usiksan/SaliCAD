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

    SdPItemPlate *getPlate() const { return mPlate; }

    SdPItemSheet *getSheet() const;

    bool          isPointInside( SdPoint p ) const;

    // SdObject interface
  public:
    virtual QString getType() const override { return QStringLiteral(SD_TYPE_AREA); }
    virtual quint64 getClass() const override { return dctArea; }
    virtual void    attach(SdUndo *undo) override;
    virtual void    detach(SdUndo *undo) override;
    virtual void    cloneFrom(const SdObject *src) override;
    virtual void    writeObject(QJsonObject &obj) const override;
    virtual void    readObject(SdObjectMap *map, const QJsonObject obj) override;

    // SdGraph interface
  public:
    virtual void    saveState(SdUndo *undo) override;
    virtual void    moveComplete( SdPoint grid, SdUndo *undo ) override;
    virtual void    move(SdPoint offset) override;
    virtual void    rotate(SdPoint center, SdAngle angle) override;
    virtual void    mirror(SdPoint a, SdPoint b) override;
    virtual void    setProp(SdProp &prop) override;
    virtual void    getProp(SdProp &prop) override;
    virtual void    selectByPoint(const SdPoint p, SdSelector *selector) override;
    virtual void    selectByRect(const SdRect &r, SdSelector *selector) override;
    virtual void    select(SdSelector *selector) override;
    virtual bool    canHideLayer(SdLayer *layer) override;
    virtual bool    isVisible() override;
    virtual SdRect  getOverRect() const override;
    virtual void    draw(SdContext *dc) override;
    virtual int     behindCursor(SdPoint p) override;
    virtual bool    snapPoint(SdSnapInfo *snap) override;
  };

#endif // SDGRAPHAREA_H
