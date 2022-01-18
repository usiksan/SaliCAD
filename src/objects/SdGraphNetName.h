/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Net name in schematic diagram
*/
#ifndef SDGRAPHWIRENAME_H
#define SDGRAPHWIRENAME_H

#include "SdGraphNet.h"
#include "SdPropText.h"
#include "SdPoint.h"

#define SD_TYPE_NET_NAME "NetName"


class SdGraphNetName : public SdGraphNet
  {
    SdPoint    mOrigin; //Place of wire name
    SdPropText mProp;   //Text properties of name
    SdRect     mOver;   //Over rect
  public:
    SdGraphNetName();
    SdGraphNetName(SdPoint org, const QString netName, const SdPropText &prp );

    // SdObject interface
  public:
    virtual QString getType() const override { return QStringLiteral(SD_TYPE_NET_NAME); }
    virtual quint64 getClass() const override { return dctNetName; }
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
    virtual void    setLayerUsage() override;
    virtual bool    isVisible() override;
    virtual SdRect  getOverRect() const override;
    virtual void    draw(SdContext *dc) override;
    virtual int     behindCursor(SdPoint p) override;
    virtual void    selectByPoint(const SdPoint p, SdSelector *selector) override;
    virtual void    selectByRect(const SdRect &r, SdSelector *selector) override;
  };

#endif // SDGRAPHWIRENAME_H
