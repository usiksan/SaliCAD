/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Text graph object
*/
#ifndef SDGRAPHTEXT_H
#define SDGRAPHTEXT_H

#include "SdGraph.h"
#include "SdPropText.h"
#include "SdRect.h"
#include <QString>

#define SD_TYPE_TEXT "Text"

class SdGraphText : public SdGraph
  {
    SdPropText mProp;
    QString    mString;
    SdRect     mOverRect;
  public:
    SdGraphText();
    SdGraphText( const QString str, SdRect r, SdPropText &p );

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
    virtual void    move(SdPoint offset) override;
    virtual void    rotate(SdPoint center, SdAngle angle) override;
    virtual void    mirror(SdPoint a, SdPoint b) override;
    virtual void    setProp(SdProp &prop) override;
    virtual void    getProp(SdProp &prop) override;
    virtual void    setText(int index, QString sour, SdPropText &prop) override;
    virtual void    selectByPoint(const SdPoint p, SdSelector *selector) override;
    virtual void    selectByRect(const SdRect &r, SdSelector *selector) override;
    virtual void    select(SdSelector *selector) override;
    virtual bool    isAble() override;
    virtual SdRect  getOverRect() const override;
    virtual void    draw(SdContext *dc) override;
    virtual int     behindCursor(SdPoint p) override;
    virtual int     behindText(SdPoint p, QString &dest, SdPropText &prop) override;
    virtual bool    getInfo(SdPoint p, QString &info, bool extInfo) override;
  };

#endif // SDGRAPHTEXT_H