/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Graphical object Arc.
*/
#ifndef SDGRAPHLINEARARC_H
#define SDGRAPHLINEARARC_H

#include "SdGraphLinear.h"
#include "SdPoint.h"
#include "SdRect.h"

#define SD_TYPE_ARC "Arc"

class SdGraphLinearArc : public SdGraphLinear
  {
//    SdPoint mCenter;
    SdRect mArcRect;  //Rect for arc's circle
    int    mStart;    //Start angle. Positive angle is counter-clock wise rotation. Zero angle is on 3 o'clock
    int    mStop;     //Stop angle. Angles is in 1/1000 degrees
  public:
    SdGraphLinearArc();
    SdGraphLinearArc( SdRect arcRect, int start, int stop );


    // SdObject interface
  public:
    virtual QString getType() const override { return QStringLiteral( SD_TYPE_ARC ); }
    virtual quint64 getClass() const override { return dctLines; }
    virtual void    cloneFrom(const SdObject *src) override;
    virtual void    writeObject(QJsonObject &obj) const override;
    virtual void    readObject(SdObjectMap *map, const QJsonObject obj) override;

    // SdGraph interface
  public:
    virtual void    saveState(SdUndo *undo) override;
    virtual void    move(SdPoint offset) override;
    virtual void rotate(SdPoint center, SdAngle angle) override;
    virtual void mirror(SdPoint a, SdPoint b) override;
    virtual void selectByPoint(const SdPoint p, SdSelector *selector) override;
    virtual void selectByRect(const SdRect &r, SdSelector *selector) override;
    virtual void select(SdSelector *selector) override;
    virtual SdRect getOverRect() const override;
    virtual void draw(SdContext *dc) override;
    virtual int behindCursor(SdPoint p) override;
    virtual bool snapPoint(SdSnapInfo *snap) override;
  };

#endif // SDGRAPHLINEARARC_H
