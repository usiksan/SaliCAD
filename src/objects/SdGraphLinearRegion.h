/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Graphical object Line Countur - Region.
*/
#ifndef SDGRAPHLINEARREGION_H
#define SDGRAPHLINEARREGION_H

#include "SdGraphLinear.h"
#include "SdPoint.h"
#include "SdPointList.h"
#include <QSet>

#define SD_TYPE_REGION "Region"


class SdGraphLinearRegion : public SdGraphLinear
  {
  protected:
    SdPointList mList;     //Region vertex points
    QSet<int>   mFlyIndex; //Fly vertex numbers
  public:
    SdGraphLinearRegion();
    SdGraphLinearRegion( const SdPointList list, const SdPropLine &propLine );

    SdPointList     getPointList() const { return mList; }

    virtual QString getType() const override { return QString(SD_TYPE_REGION); }
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
    virtual void    select(SdSelector *selector) override;
    virtual SdRect  getOverRect() const override;
    virtual void    draw(SdContext *dc) override;

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
  };

#endif // SDGRAPHLINEARREGION_H
