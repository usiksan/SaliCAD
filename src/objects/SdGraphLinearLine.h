/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Graphical object Line. Line is linear segment with two ends.
*/
#ifndef SDGRAPHLINEARLINE_H
#define SDGRAPHLINEARLINE_H

#include "SdGraphLinear.h"
#include "SdPoint.h"

#define SD_TYPE_LINE "Line"

class SdGraphLinearLine : public SdGraphLinear
  {
  protected:
    SdPoint a,b;
    bool    mFlyA;
    bool    mFlyB;
  public:
    SdGraphLinearLine();
    SdGraphLinearLine( SdPoint p1, SdPoint p2, const SdPropLine &propLine );

    // SdObject interface
  public:
    virtual QString getType() const override;
    virtual quint64 getClass() const override;
    virtual void    cloneFrom(const SdObject *src) override;
    virtual void    writeObject(QJsonObject &obj) const override;
    virtual void    readObject(SdObjectMap *map, const QJsonObject obj) override;
    virtual void    move(SdPoint offset) override;
    virtual void    rotate(SdPoint center, SdPropAngle angle) override;
    virtual void    mirror(SdPoint a, SdPoint b) override;
    virtual void    selectByPoint(const SdPoint p, SdSelector *selector) override;
    virtual void    selectByRect(const SdRect &r, SdSelector *selector) override;
    virtual void    select(SdSelector *selector) override;
    virtual SdRect  getOverRect() const override;
    virtual void    draw(SdContext *dc) override;

    //!
    //! \brief draw3d Draws object in 3d space
    //! \param f      3d draw functions with predefined 3d context
    //!
    virtual void    draw3d( QOpenGLFunctions_2_0 *f ) const override;

    //Find snap point on object
    virtual void    snapPoint(SdSnapInfo *snap) override;
    virtual int     behindCursor(SdPoint p) override;
    virtual void    saveState( SdUndo *undo ) override;

    friend class SdPasCadImport;
  };

#endif // SDGRAPHLINEARLINE_H
