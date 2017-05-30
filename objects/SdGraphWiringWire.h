/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/
#ifndef SDGRAPHWIRINGWIRE_H
#define SDGRAPHWIRINGWIRE_H

#include "SdGraphWiring.h"
#include "SdPropLine.h"
#include "SdPoint.h"

#define SD_TYPE_WIRE "Wire"

class SdContainerSheetNet;

class SdGraphWiringWire : public SdGraphWiring
  {
    SdPoint    mA,mB;       //Wire segment
    SdPropLine mProp;       //Wire drawing properties
    bool       mDotA,mDotB; //Dots present flag

    const int vdNone = 0, vdX = 1, vdY = 2;
    int        mDir;
    bool       mFix;
  public:
    SdGraphWiringWire();
    SdGraphWiringWire( SdPoint a, SdPoint b, const SdPropLine &prp );

    //Convert parent to net
    SdContainerSheetNet*  getNet() const;

    //Get wire name
    QString               getWireName() const;

    SdPoint getA() const { return mA; }
    SdPoint getB() const { return mB; }
    bool    getDotA() const { return mDotA; }
    bool    getDotB() const { return mDotB; }
    void    accumLinked( SdPoint a, SdPoint b, SdSelector *sel, SdUndo *undo );
//    void   Union( DWirePic *segment );
    bool    isPointOnSection( SdPoint p ) const { return p.isOnSegment( mA, mB ); }
//    void   Utilise();
//    void   SetDotPoint();
    void    drawWire( SdContext *dc, QColor color );
  protected:
//    void CalcVertexPoints( DPoint &p1, DPoint &p2, DPoint gridSize );
//    void Exchange();
    void    fragmentation( SdPoint p, SdSelector *sel, SdUndo *undo );
//    bool GetNeedDot( DPoint p );

    // SdObject interface
  public:
    virtual QString getType() const override { return QStringLiteral(SD_TYPE_WIRE); }
    virtual quint64 getClass() const override { return dctWire; }
    virtual void attach(SdUndo *undo) override;
    virtual void detach(SdUndo *undo) override;
    virtual void cloneFrom(const SdObject *src) override;
    virtual void writeObject(QJsonObject &obj) const override;
    virtual void readObject(SdObjectMap *map, const QJsonObject obj) override;

    // SdGraph interface
  public:
    virtual void saveState(SdUndo *undo) override;
    virtual void moveComplete(SdPoint grid, SdUndo *undo) override;
    virtual void move(SdPoint offset) override;
    virtual void rotate(SdPoint center, SdAngle angle) override;
    virtual void mirror(SdPoint a, SdPoint b) override;
    virtual void setProp(SdProp &prop) override;
    virtual void getProp(SdProp &prop) override;
    virtual void selectByPoint(const SdPoint p, SdSelector *selector) override;
    virtual void selectByRect(const SdRect &r, SdSelector *selector) override;
    virtual void select(SdSelector *selector) override;
    virtual void prepareMove() override;
    virtual bool canHideLayer(SdLayer *layer) override;
    virtual bool isVisible() override;
    virtual SdRect getOverRect() const override;
    virtual void draw(SdContext *dc) override;
    virtual int behindCursor(SdPoint p) override;
    virtual bool getInfo(SdPoint p, QString &info, bool extInfo) override;
    virtual bool snapPoint(SdSnapInfo *snap) override;

    // SdGraphWiring interface
  public:
    virtual bool getNetOnPoint(SdPoint p, QString &destName) override;
  };

#endif // SDGRAPHWIRINGWIRE_H
