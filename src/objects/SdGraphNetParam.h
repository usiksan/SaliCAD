/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Net param in schematic diagram
*/
#ifndef SDGRAPHNETPARAM_H
#define SDGRAPHNETPARAM_H

#include "SdGraphNet.h"
#include "SdPropText.h"
#include "SdPoint.h"

#define SD_TYPE_NET_PARAM "NetParam"

class SdGraphNetParam : public SdGraphNet
  {
    SdPoint    mOrigin;    //!< Place of wire name
    QString    mParam;     //!< Param in form of "ParamName = ParamValue"
    SdPropText mProp;      //!< Text properties of name
    SdRect     mOver;      //!< Over rect
  public:
    SdGraphNetParam();
    SdGraphNetParam( SdPoint org, const QString &netName, const QString &param, const SdPropText &prp );

    QString paramName() const;

    QString paramValueGet() const;

    void    paramValueSet( const QString &value );

    QString paramText() const { return mParam; }

  private:
    void    paramParse( const QString &param );

    // SdObject interface
  public:
    virtual QString getType() const override { return QStringLiteral(SD_TYPE_NET_PARAM); }
    virtual quint64 getClass() const override { return dctNetParam; }
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
    virtual void    setText(int index, QString sour, SdPropText &prop, QWidget *parent) override;
    virtual void    selectByPoint(const SdPoint p, SdSelector *selector) override;
    virtual void    selectByRect(const SdRect &r, SdSelector *selector) override;
    virtual bool    isVisible() override;
    virtual SdRect  getOverRect() const override;
    virtual void    draw(SdContext *dc) override;
    virtual int     behindCursor(SdPoint p) override;
    virtual int     behindText(SdPoint p, SdPoint &org, QString &dest, SdPropText &prop) override;
  };

#endif // SDGRAPHNETPARAM_H
