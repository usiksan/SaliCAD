#ifndef SD3DFLAT_H
#define SD3DFLAT_H

#include "Sd3dPoint.h"
#include "Sd3dGraph.h"

#include <QList>

#define SD_TYPE_3D_FLAT "3dFlat"


using SdPoint3dList = QList<Sd3dPoint>;

class Sd3dFlat : public Sd3dGraph
  {
    SdPoint3dList mRegion;
    Sd3dPoint     mWidthVector;
    quint32       mColor;
  public:
    Sd3dFlat();

    void buildParallelepiped( int x, int y, int z, unsigned color );

    // SdObject interface
  public:
    virtual QString getType() const override;
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

    // Sd3dObject interface
  public:
    virtual void    draw3d(QOpenGLFunctions_2_0 *f) const override;
  };

#endif // SD3DFLAT_H
