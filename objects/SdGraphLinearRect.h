/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Graphical object rectangle.
*/
#ifndef SDGRAPHLINEARRECT_H
#define SDGRAPHLINEARRECT_H

#include "SdGraphLinear.h"

#define SD_TYPE_RECT "Rect"

class SdGraphLinearRect : public SdGraphLinear
  {
  protected:
    SdPoint a,b;  //Координаты
    int     mFly; //Номер плавающей вершины (или перенос всего прямоугольника)
  public:
    SdGraphLinearRect();
    SdGraphLinearRect( SdPoint p1, SdPoint p2, const SdPropLine &propLine );


    // SdObject interface
    virtual QString getType() const override;
    virtual quint64 getClass() const override;
    virtual void    cloneFrom(const SdObject *src) override;
    virtual void    writeObject(QJsonObject &obj) const override;
    virtual void    readObject(SdObjectMap *map, const QJsonObject obj) override;

    // SdGraph interface
    virtual void    move(SdPoint offset) override;
    virtual void    rotate(SdPoint center, SdAngle angle) override;
    virtual void    mirror(SdPoint a, SdPoint b) override;
    virtual void    selectByPoint(const SdPoint p, SdSelector *selector) override;
    virtual void    selectByRect(const SdRect &r, SdSelector *selector) override;
    virtual void    select(SdSelector *selector) override;
    virtual SdRect  getOverRect() const override;
    virtual void    draw(SdContext *dc) override;
    virtual int     behindCursor(SdPoint p) override;
    virtual bool    snapPoint(SdSnapInfo *snap) override;

    //Save object state
    virtual void    saveState( SdUndo *undo ) override;
  };

#endif // SDGRAPHLINEARRECT_H
