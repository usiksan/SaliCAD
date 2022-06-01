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

#include "Sd3dGraph.h"

#include <QList>
#include <QDataStream>
#include <QJsonObject>

#if 0
#define SD_TYPE_3D_STEP "3dStep"


class Sd3dStep : public Sd3dGraph
  {
//    Sd3dFaceList mFaceList; //!< Face list of model
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
  public:
    virtual SdRect  getOverRect() const override;
    virtual void    draw3d(QOpenGLFunctions_2_0 *f) const override;

  private:
    //!
    //! \brief importStepStyledFace Reads one face from step file reader
    //! \param faceId               Face id for reading
    //! \param reader               STEP file reader
    //!
    //bool importStepStyledFace(const QString &faceId, const SdStepReader &reader);
  };

#endif

#endif // SD3DSTEP_H
