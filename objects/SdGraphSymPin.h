/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Graphical object Symbol pin.
*/
#ifndef SDGRAPHSYMPIN_H
#define SDGRAPHSYMPIN_H

#include "SdGraph.h"
#include "SdPoint.h"
#include "SdPropText.h"
#include "SdPropSymPin.h"

#define SD_TYPE_SYM_PIN "SymPin"

class SdGraphSymPin : public SdGraph
  {
    SdPoint      mOrigin;     //Pin origin
    SdPropSymPin mPinProp;    //Pin properties
    SdPoint      mNumberPos;  //Pin number position
    SdRect       mNumberRect; //Pin number over rect
    SdPropText   mNumberProp; //Pin number properties
    SdPoint      mNamePos;    //Pin name position
    SdRect       mNameRect;   //Pin name over rect
    SdPropText   mNameProp;   //Pin name properties
    QString      mName;       //Pin name

    //Different pin part selection
    bool         mPinSelect;  //Pin selected
    bool         mNumSelect;  //Pin number selected
    bool         mNamSelect;  //Pin name selected
  public:
    SdGraphSymPin();
    SdGraphSymPin( SdPoint org, SdPoint numberPos, SdPoint namePos, const QString name );

    QString  getPinName() const { return mName; }
    SdPoint  getPinOrigin() const { return mOrigin; }
    //Draw pin in symbol implementation
    void     drawImp( SdContext *dc, const QString pinNumber, bool com );

    // SdObject interface
  public:
    virtual QString   getType() const override { return QStringLiteral( SD_TYPE_SYM_PIN ); }
    virtual quint64   getClass() const override { return dctSymPin; }
    virtual SdObject *copyNext() override;
    virtual void      cloneFrom(const SdObject *src) override;
    virtual void      writeObject(QJsonObject &obj) const override;
    virtual void      readObject(SdObjectMap *map, const QJsonObject obj) override;

    // SdGraph interface
  public:
    virtual void      saveState(SdUndo *undo) override;
    virtual void      move(SdPoint offset) override;
    virtual void      rotate(SdPoint center, SdAngle angle) override;
    virtual void      mirror(SdPoint a, SdPoint b) override;
    virtual void      setProp(SdProp &prop) override;
    virtual void      getProp(SdProp &prop) override;
    virtual void      setText(int index, QString sour, SdPropText &prop, QWidget *parent) override;
    virtual void      selectByPoint(const SdPoint p, SdSelector *selector) override;
    virtual void      selectByRect(const SdRect &r, SdSelector *selector) override;
    virtual void      select(SdSelector *selector) override;
    virtual bool      isVisible() override;
    virtual SdRect    getOverRect() const override;
    virtual void      draw(SdContext *dc) override;
    virtual int       behindCursor(SdPoint p) override;
    virtual int       behindText( SdPoint p, SdPoint &org, QString &dest, SdPropText &prop ) override;
    virtual bool      snapPoint(SdSnapInfo *snap) override;
  };

#endif // SDGRAPHSYMPIN_H
