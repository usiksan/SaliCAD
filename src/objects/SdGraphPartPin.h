/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Graphical object Part pin.
*/
#ifndef SDGRAPHPARTPIN_H
#define SDGRAPHPARTPIN_H

#include "SdGraph.h"
#include "SdPoint.h"
#include "SdPropText.h"
#include "SdPropPartPin.h"
#include "SdStratum.h"

#define SD_TYPE_PART_PIN "PartPin"

class SdPItemPlate;

class SdGraphPartPin : public SdGraph
  {
    SdPoint       mOrigin;     //Pin origin
    SdPropPartPin mPinProp;    //Pin properties
    SdPoint       mNamePos;    //Pin name position
    SdRect        mNameRect;   //Pin name over rect
    SdPropText    mNameProp;   //Pin name properties
    SdPoint       mNumberPos;  //Pin number position
    SdRect        mNumberRect; //Pin number over rect
    SdPropText    mNumberProp; //Pin number properties
    QString       mNumber;     //Pin number

    //Different pin part selection
    bool          mPinSelect;  //Pin selected
    bool          mNumSelect;  //Pin number selected
    bool          mNamSelect;  //Pin name selected
  public:
    SdGraphPartPin();
    SdGraphPartPin( SdPoint org, SdPoint numberPos, SdPoint namePos, const QString number );
    SdGraphPartPin( SdPoint org, const SdPropPartPin &pinProp, SdPoint numberPos, const SdPropText &numberProp, SdPoint namePos, const SdPropText &nameProp, const QString number );

    //Return pin number
    QString   getPinNumber() const { return mNumber; }
    //Return pin type
    QString   getPinType() const { return mPinProp.mPinType.str(); }
    //Return pin origin
    SdPoint   getPinOrigin() const { return mOrigin; }
    //Return pin stratum
    int       getPinStratum( bool otherSide ) const;

    void      drawImp(SdContext *dc, const QString pinName, bool com );

    // SdObject interface
  public:
    virtual QString   getType() const override { return QString( SD_TYPE_PART_PIN ); }
    virtual quint64   getClass() const override { return dctPartPin; }
    virtual SdObject *copyNext() const override;
    virtual void      cloneFrom(const SdObject *src) override;
    virtual void      writeObject(QJsonObject &obj) const override;
    virtual void      readObject(SdObjectMap *map, const QJsonObject obj) override;

    // SdGraph interface
  public:
    virtual void      saveState(SdUndo *undo) override;
    virtual void      move(SdPoint offset) override;
    virtual void      rotate(SdPoint center, SdPropAngle angle) override;
    virtual void      mirror(SdPoint a, SdPoint b) override;
    virtual void      setProp(SdPropSelected &prop) override;
    virtual void      getProp(SdPropSelected &prop) override;
    virtual void      setText(int index, QString sour, SdPropText &prop, QWidget *parent) override;
    virtual void      selectByPoint(const SdPoint p, SdSelector *selector) override;
    virtual void      selectByRect(const SdRect &r, SdSelector *selector) override;
    virtual void      select(SdSelector *selector) override;
    virtual bool      isVisible() override;
    virtual SdRect    getOverRect() const override;
    virtual void      draw(SdContext *dc) override;
    virtual int       behindCursor(SdPoint p) override;
    virtual int       behindText( SdPoint p, SdPoint &org, QString &dest, SdPropText &prop ) override;
    //Find snap point on object
    virtual void      snapPoint(SdSnapInfo *snap) override;

    friend class SdPasCadImport;
  };

#endif // SDGRAPHPARTPIN_H