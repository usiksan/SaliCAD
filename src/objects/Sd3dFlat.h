#ifndef SD3DFLAT_H
#define SD3DFLAT_H

#include "SdPoint3d.h"
#include "Sd3dObject.h"

#include <QList>

#define SD_TYPE_3D_FLAT "3dFlat"


using SdPoint3dList = QList<SdPoint3d>;

class Sd3dFlat : public Sd3dObject
  {
    SdPoint3dList mRegion;
    SdPoint3d     mWidthVector;
    quint32       mColor;
  public:
    Sd3dFlat();

    void buildParallelepiped( int x, int y, int z, unsigned color );

    // SdObject interface
  public:
    virtual QString getType() const override;
    virtual void    cloneFrom(const SdObject *src) override;
    virtual void    writeObject(QJsonObject &obj) const override;
    virtual void    readObject(SdObjectMap *map, const QJsonObject obj) override;

    // Sd3dObject interface
  public:
    virtual void    draw(QOpenGLFunctions_2_0 *f) override;
  };

#endif // SD3DFLAT_H
