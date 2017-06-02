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

#include "SdGraph.h"
#include "SdPropText.h"
#include "SdPoint.h"

#define SD_TYPE_WIRE_NAME "WireName"

class SdContainerSheetNet;


class SdGraphWireName : public SdGraph
  {
    SdPoint    mOrigin; //Place of wire name
    SdPropText mProp;   //Text properties of name
    SdRect     mOver;   //Over rect
  public:
    SdGraphWireName();
    SdGraphWireName( SdPoint org, const SdPropText &prp );

    //Get owner net
    SdContainerSheetNet *getNet() const;
    //Get owner net name
    QString              getNetName() const;

    // SdObject interface
  public:
    virtual QString getType() const override { return QStringLiteral(SD_TYPE_WIRE_NAME); }
    virtual quint64 getClass() const override { return dctWireName; }
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
    virtual bool    canHideLayer(SdLayer *layer) override;
    virtual bool    isVisible() override;
    virtual SdRect  getOverRect() const override;
    virtual void    draw(SdContext *dc) override;
    virtual int     behindCursor(SdPoint p) override;
    virtual void    selectByPoint(const SdPoint p, SdSelector *selector) override;
    virtual void    selectByRect(const SdRect &r, SdSelector *selector) override;
    virtual void    select(SdSelector *selector) override;
  };

#endif // SDGRAPHWIRENAME_H
