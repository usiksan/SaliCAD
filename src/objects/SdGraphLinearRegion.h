/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Graphical object Line Countur - Region.
*/
#ifndef SDGRAPHLINEARREGION_H
#define SDGRAPHLINEARREGION_H

#include "SdGraphLinear.h"
#include "SdPoint.h"
#include "SdPointList.h"
#include <QSet>

#define SD_TYPE_REGION "Region"


class SdGraphLinearRegion : public SdGraphLinear
  {
  protected:
    SdPointList mList;     //Region vertex points
    QSet<int>   mFlyIndex; //Fly vertex numbers
  public:
    SdGraphLinearRegion();
    SdGraphLinearRegion( const SdPointList list, const SdPropLine &propLine );

    SdPointList     getPointList() const { return mList; }

    virtual QString getType() const override { return QString(SD_TYPE_REGION); }
    virtual quint64 getClass() const override { return dctLines; }
    virtual void    cloneFrom(const SdObject *src) override;
    virtual void    writeObject(QJsonObject &obj) const override;
    virtual void    readObject(SdObjectMap *map, const QJsonObject obj) override;
    virtual void    move(SdPoint offset) override;
    virtual void    rotate(SdPoint center, SdPropAngle angle) override;
    virtual void    mirror(SdPoint a, SdPoint b) override;
    virtual void    selectByPoint(const SdPoint p, SdSelector *selector) override;
    virtual void    selectByRect(const SdRect &r, SdSelector *selector) override;
    virtual void    select(SdSelector *selector) override;
    virtual SdRect  getOverRect() const override;
    virtual void    draw(SdContext *dc) override;
    //Find snap point on object
    virtual void    snapPoint(SdSnapInfo *snap) override;
    virtual int     behindCursor(SdPoint p) override;
    virtual void    saveState( SdUndo *undo ) override;
  };

#endif // SDGRAPHLINEARREGION_H
