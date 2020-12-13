/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  3d object which represents stl model
*/
#ifndef SD3DSTL_H
#define SD3DSTL_H

#include "Sd3dObject.h"
#include "Sd3dTriangle.h"

#include <QList>
#include <QDataStream>
#include <QJsonObject>

#define SD_TYPE_3D_STL "3dStl"

using Sd3dTriangleList = QList<Sd3dTriangle>;

class Sd3dStl : public Sd3dObject
  {
    Sd3dTriangleList  mTriangleList; //!< Triangle list of model
  public:
    Sd3dStl();

    //!
    //! \brief importStl Read model from STL file
    //! \param file      STL file
    //! \result          true if import successfull or false in other case
    //!
    bool importStl( QIODevice *file );

    //!
    //! \brief importStlFromFile Read model from STL file which represented by its path
    //! \param fname             Full path to STL file
    //! \return                  Pointer to Sd3dStl object if import was successfull or nullptr in other case
    //!
    static Sd3dStl *importStlFromFile( QString fname );

    // SdObject interface
  public:
    virtual QString getType() const override { return QStringLiteral( SD_TYPE_3D_STL ); }
    virtual void    writeObject(QJsonObject &obj) const override;
    virtual void    readObject(SdObjectMap *map, const QJsonObject obj) override;

    // SdGraph interface
  public:
    virtual SdRect  getOverRect() const override;
    virtual void    draw3d(QOpenGLFunctions_2_0 *f) const override;
  };

#endif // SD3DSTL_H
