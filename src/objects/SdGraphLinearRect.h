/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Graphical object rectangle.
*/
#ifndef SDGRAPHLINEARRECT_H
#define SDGRAPHLINEARRECT_H

#include "SdGraphLinear.h"

#define SD_TYPE_RECT "Rect"

class SdGraphLinearRect : public SdGraphLinear
  {
  protected:
    SdPoint a,b;  //Координаты
    int     mFly; //Номер плавающей вершины (или перенос всего прямоугольника)
  public:
    SdGraphLinearRect();
    SdGraphLinearRect( SdPoint p1, SdPoint p2, const SdPropLine &propLine );


    SdPointList     getPointList() const;

    // SdObject interface
    virtual QString getType() const override;
    virtual quint64 getClass() const override;
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

    // SdGraph interface
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

    //!
    //! \brief accumHoles Accum holes description into faceList
    //! \param model      Model which accumulate coord vertexes
    //! \param faceList   Face list for holding holes (single face for single hole)
    //! \param stratum    Stratum for layers
    //! \param map        Map for holes conversion
    //!
    virtual void    accumHoles( Sd3drModel &model, Sd3drFaceList &faceList, SdStratum stratum, const QMatrix4x4 &map ) const override;

    virtual int     behindCursor(SdPoint p) override;
    //Find snap point on object
    virtual void    snapPoint(SdSnapInfo *snap) override;

    //Save object state
    virtual void    saveState( SdUndo *undo ) override;

    friend class SdPasCadImport;
  };

#endif // SDGRAPHLINEARRECT_H
