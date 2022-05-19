/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  One segment of wire in schematic diagram
*/
#ifndef SDGRAPHWIRINGWIRE_H
#define SDGRAPHWIRINGWIRE_H

#include "SdGraphNet.h"
#include "SdPropLine.h"
#include "SdPoint.h"

#define SD_TYPE_NET_WIRE "NetWire"

class SdGraphNetWire : public SdGraphNet
  {
    SdPoint    mA,mB;       //Wire segment
    SdPropLine mProp;       //Wire drawing properties
    bool       mDotA,mDotB; //Dots present flag

    enum { vdNone = 0, vdX = 1, vdY = 2 };
    int        mDirX;
    bool       mFixA;
    bool       mFixB;
  public:
    SdGraphNetWire();
    SdGraphNetWire(SdPoint a, SdPoint b, const QString netName, const SdPropLine &prp );

    SdPoint getA() const { return mA; }
    SdPoint getB() const { return mB; }
    bool    getDotA() const { return mDotA; }
    bool    getDotB() const { return mDotB; }
    void    accumLinked( SdPoint a, SdPoint b, SdSelector *sel, SdUndo *undo );
    void    unionSegments(SdGraphNetWire *segment , SdUndo *undo);
    bool    isPointOnSection( SdPoint p ) const { return p.isOnSegment( mA, mB ); }
    void    utilise( SdUndo *undo );
    //bool    getNetOnPoint(SdPoint p, QString &destName);
  protected:
    void    calcVertexPoints( SdPoint &p1, SdPoint &p2, SdPoint gridSize ) const;
    void    fragmentation( SdPoint p, SdSelector *sel, SdUndo *undo );
    bool    getNeedDot( SdPoint a, SdPoint b);
    SdPoint getFixPoint(SdPoint a, SdPoint b);

    // SdObject interface
  public:
    virtual QString getType() const override { return QStringLiteral(SD_TYPE_NET_WIRE); }
    virtual quint64 getClass() const override { return dctNetWire; }
    virtual void    attach(SdUndo *undo) override;
    virtual void    detach(SdUndo *undo) override;
    //!
    //! \brief cloneFrom Overrided function. We copy object from source
    //! \param src       Source of object from which copy must be made
    //! \param copyMap   Structure for mapping copying substitutes
    //! \param next      Make simple or next copy. Next copy available not for all objects.
    //!                  For example: pin name A23 with next copy return A24
    //!
    virtual void    cloneFrom( const SdObject *src, SdCopyMap &copyMap, bool next ) override;
    virtual void    writeObject(QJsonObject &obj) const override;
    virtual void    readObject(SdObjectMap *map, const QJsonObject obj) override;

    // SdGraph interface
  public:
    virtual void    saveState(SdUndo *undo) override;
    virtual void    moveComplete(SdPoint grid, SdUndo *undo) override;
    virtual void    move(SdPoint offset) override;
    virtual void    rotate(SdPoint center, SdPropAngle angle) override;
    virtual void    mirror(SdPoint a, SdPoint b) override;
    virtual void    setProp(SdPropSelected &prop) override;
    virtual void    getProp(SdPropSelected &prop) override;
    virtual void    selectByPoint(const SdPoint p, SdSelector *selector) override;
    virtual void    selectByRect(const SdRect &r, SdSelector *selector) override;
    virtual void    select(SdSelector *selector) override;
    virtual void    prepareMove( SdUndo *undo ) override;
    virtual void    setLayerUsage() override;
    virtual bool    isVisible() override;
    virtual SdRect  getOverRect() const override;
    virtual void    draw(SdContext *dc) override;
    virtual int     behindCursor(SdPoint p) override;
    virtual bool    getInfo(SdPoint p, QString &info, bool extInfo) override;
    //Find snap point on object
    virtual void    snapPoint(SdSnapInfo *snap) override;

    // SdGraphNet interface
  public:
    virtual void setNetName(const QString netName, SdUndo *undo) override;
  };

#endif // SDGRAPHWIRINGWIRE_H
