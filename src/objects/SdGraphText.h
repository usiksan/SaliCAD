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
  protected:
    SdPoint    mOrigin;
    SdPropText mProp;
    QString    mString;
    SdRect     mOverRect;
  public:
    SdGraphText();
    SdGraphText( SdPoint org, const QString &str, SdRect r, const SdPropText &p );

    //Information
    QString         getText() const { return mString; }
    SdPoint         getOrigin() const { return mOrigin; }
    SdPropText      getPropText() const { return mProp; }

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
    virtual void    rotate(SdPoint center, SdPropAngle angle) override;
    virtual void    mirror(SdPoint a, SdPoint b) override;
    virtual void    setProp(SdPropSelected &prop) override;
    virtual void    getProp(SdPropSelected &prop) override;
    virtual void    setText(int index, QString sour, SdPropText &prop, QWidget *parent) override;
    virtual void    selectByPoint(const SdPoint p, SdSelector *selector) override;
    virtual void    selectByRect(const SdRect &r, SdSelector *selector) override;
    virtual bool    isVisible() override;
    virtual SdRect  getOverRect() const override;
    virtual void    draw(SdContext *dc) override;
    virtual int     behindCursor(SdPoint p) override;
    virtual int     behindText(SdPoint p, SdPoint &org, QString &dest, SdPropText &prop) override;

    friend class SdPasCadImport;
  };

#endif // SDGRAPHTEXT_H
