/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  3d object which represents step model
*/
#ifndef SD3DSTEP_H
#define SD3DSTEP_H

#include "Sd3dObject.h"
#include "Sd3dFace.h"

#include <QList>
#include <QDataStream>
#include <QJsonObject>

#define SD_TYPE_3D_STEP "3dStep"

using Sd3dFaceList = QList<Sd3dFace>;

class Sd3dStep : public Sd3dObject
  {
    Sd3dFaceList mFaceList; //! Face list of model
  public:
    Sd3dStep();

    //!
    //! \brief importStep Read model from STEP file
    //! \param file       STEP file
    //! \result           true if import successfull or false in other case
    //!
    bool importStep( QIODevice *file );

    //!
    //! \brief importStepFromFile Read model from STEP file which represented by its path
    //! \param fname              Full path to STEP file
    //! \return                   Pointer to Sd3dStep object if import was successfull or nullptr in other case
    //!
    static Sd3dStep *importStepFromFile( QString fname );

    // SdObject interface
  public:
    virtual QString getType() const override { return QStringLiteral( SD_TYPE_3D_STEP ); }
    virtual void    writeObject(QJsonObject &obj) const override;
    virtual void    readObject(SdObjectMap *map, const QJsonObject obj) override;

    // SdGraph interface
  public:
    virtual SdRect  getOverRect() const override;
    virtual void    draw3d(QOpenGLFunctions_2_0 *f) const override;

  };

#endif // SD3DSTEP_H
