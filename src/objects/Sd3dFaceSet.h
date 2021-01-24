/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  3d object which represents face set model (Model which consists of face set)
  Each face is flate surface bounded polyline
*/
#ifndef SD3DFACESET_H
#define SD3DFACESET_H

#include "Sd3dObject.h"
#include "Sd3dFace.h"

#include <QList>
#include <QDataStream>
#include <QJsonObject>

#define SD_TYPE_3D_FACE_SET "3dFaceSet"

using Sd3dFaceList = QList<Sd3dFace>;

class Sd3dFaceSet : public Sd3dObject
  {
    Sd3dFaceList mFaceList; //!< Face list of model
  public:
    Sd3dFaceSet();

    //!
    //! \brief faceAdd Appends face to face list of model
    //! \param face    Appended face
    //!
    void faceAdd( const Sd3dFace &face );
    // SdObject interface
  public:
    virtual QString getType() const override { return QStringLiteral( SD_TYPE_3D_FACE_SET ); }
    virtual void    writeObject(QJsonObject &obj) const override;
    virtual void    readObject(SdObjectMap *map, const QJsonObject obj) override;

    // SdGraph interface
  public:
    virtual SdRect  getOverRect() const override;
    virtual void    draw3d(QOpenGLFunctions_2_0 *f) const override;
  };

#endif // SD3DFACESET_H
