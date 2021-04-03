/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  3d object in object hierarchy which represents face set model (Model which consists of face set)
  Each face is flate surface bounded polyline
  With comparison Sd3dFaceSet this object contains simplified faces (no normal, single face color)
*/
#ifndef SD3DGRAPHMODEL_H
#define SD3DGRAPHMODEL_H

#include "Sd3dModel.h"
#include "Sd3dGraph.h"

#define SD_TYPE_3D_GRAPH_MODEL "3dGraphModel"

class Sd3dGraphModel : public Sd3dGraph
  {
    Sd3dModel mModel; //!< 3d model (face set)
  public:
    Sd3dGraphModel();

    //!
    //! \brief faceAdd Appends face to 3d model
    //! \param face    Appended face
    //!
    void faceAdd( const Sd3dFace &face ) { mModel.append( face ); }

    //!
    //! \brief modelAdd Append model to 3d model
    //! \param model    Appended model
    //!
    void modelAdd( const Sd3dModel &model ) { mModel.append( model ); }

    //!
    //! \brief matrixMapInPlace Mapping all faces of model throught given matrix. Operation maked in place
    //! \param matrix           Mapping matrix
    //!
    void matrixMapInPlace( QMatrix4x4 matrix );

    //!
    //! \brief volumeAdd Append volume of model to result volume
    //! \param volume    Source and result volume
    //!
    virtual void    volumeAdd( QMatrix2x3 &volume ) const override { sd3dModelVolume( mModel, volume ); }

    // SdObject interface
  public:
    virtual QString getType() const override { return QStringLiteral( SD_TYPE_3D_GRAPH_MODEL ); }
    virtual void    writeObject(QJsonObject &obj) const override;
    virtual void    readObject(SdObjectMap *map, const QJsonObject obj) override;
    virtual void    cloneFrom(const SdObject *src) override;

    // SdGraph interface
  public:
    virtual SdRect  getOverRect() const override;
    virtual void    draw3d(QOpenGLFunctions_2_0 *f) const override;

  };

#endif // SD3DGRAPHMODEL_H
