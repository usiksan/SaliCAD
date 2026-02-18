/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Graphical object Circle.
*/
#ifndef SDGRAPHLINEARCIRCLE_H
#define SDGRAPHLINEARCIRCLE_H

#include "SdGraphLinear.h"
#include "SdPoint.h"

#define SD_TYPE_CIRCLE "Circle"


class SdGraphLinearCircle : public SdGraphLinear
  {
  protected:
    SdPoint mCenter;  //Center of circle
    int     mRadius;  //Radius of circle
  public:
    SdGraphLinearCircle();
    SdGraphLinearCircle( SdPoint center, int radius, const SdPropLine &propLine );

    // SdObject interface
  public:
    virtual QString getType() const override { return QStringLiteral(SD_TYPE_CIRCLE); }
    virtual quint64 getClass() const override { return dctLines; }

    //!
    //! \brief cloneFrom Overrided function. We copy object from source
    //! \param src       Source of object from which copy must be made
    //! \param copyMap   Structure for mapping copying substitutes
    //! \param next      Make simple or next copy. Next copy available not for all objects.
    //!                  For example: pin name A23 with next copy return A24
    //!
    virtual void    cloneFrom( const SdObject *src, SdCopyMap &copyMap, bool next ) override;

    //!
    //! \brief json Overloaded function to write object content into json writer
    //!             Overrided function
    //! \param js   Json writer
    //!
    virtual void    json( SdJsonWriter &js ) const override;

    //!
    //! \brief json Overloaded function to read object content from json reader
    //!             Overrided function
    //! \param js   Json reader
    //!
    virtual void    json( const SdJsonReader &js ) override;

    virtual void    transform( const QTransform &map, SdPvAngle ) override;
    virtual void    selectByPoint(const SdPoint p, SdSelector *selector) override;
    virtual void    selectByRect(const SdRect &r, SdSelector *selector) override;
    virtual SdRect  getOverRect() const override;
    virtual void    draw(SdContext *dc) override;

    //!
    //! \brief draw3d Draws object in 3d space
    //! \param f      3d draw functions with predefined 3d context
    //!
    virtual void    draw3d( QOpenGLFunctions_2_0 *f ) const override;

    //!
    //! \brief accumHoles Accum holes description into faceList
    //! \param model      Model which accumulate coord vertexes
    //! \param faceList   Face list for holding holes (single face for single hole)
    //! \param stratum    Stratum for layers
    //! \param map        Map for holes conversion
    //!
    virtual void    accumHoles( Sd3drModel &model, Sd3drFaceList &faceList, SdPvStratum stratum, const QMatrix4x4 &map ) const override;

    //Find snap point on object
    virtual void    snapPoint(SdSnapInfo *snap) override;
    virtual int     behindCursor(SdPoint p) override;
    virtual void    saveState( SdUndo *undo ) override;

    friend class SdPasCadImport;
  };

#endif // SDGRAPHLINEARCIRCLE_H
