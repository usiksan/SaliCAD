/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Graphical object Circle.
*/
#ifndef SDGRAPHLINEARCIRCLE_H
#define SDGRAPHLINEARCIRCLE_H

#include "SdGraphLinear.h"
#include "SdPoint.h"

#define SD_TYPE_CIRCLE "Circle"


class SdGraphLinearCircle : public SdGraphLinear
  {
  protected:
    SdPoint mCenter;  //Center of circle
    int     mRadius;  //Radius of circle
  public:
    SdGraphLinearCircle();
    SdGraphLinearCircle( SdPoint center, int radius, const SdPropLine &propLine );
    // SdObject interface
  public:
    virtual QString getType() const override { return QStringLiteral(SD_TYPE_CIRCLE); }
    virtual quint64 getClass() const override { return dctLines; }
    virtual void    cloneFrom(const SdObject *src) override;
    virtual void    writeObject(QJsonObject &obj) const override;
    virtual void    readObject(SdObjectMap *map, const QJsonObject obj) override;
    virtual void    move(SdPoint offset) override;
    virtual void    rotate(SdPoint center, SdPropAngle angle) override;
    virtual void    mirror(SdPoint a, SdPoint b) override;
    virtual void    selectByPoint(const SdPoint p, SdSelector *selector) override;
    virtual void    selectByRect(const SdRect &r, SdSelector *selector) override;
    virtual SdRect  getOverRect() const override;
    virtual void    draw(SdContext *dc) override;
    virtual bool    snapPoint(SdSnapInfo *snap) override;
    virtual int     behindCursor(SdPoint p) override;
    virtual void    saveState( SdUndo *undo ) override;

    friend class SdPasCadImport;
  };

#endif // SDGRAPHLINEARCIRCLE_H
