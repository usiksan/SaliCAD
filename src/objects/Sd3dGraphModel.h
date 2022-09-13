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

#include "Sd3dGraph.h"
#include "3dr/Sd3drModel.h"

#define SD_TYPE_3D_GRAPH_MODEL "3dGraphModel"

class Sd3dGraphModel : public Sd3dGraph
  {
    Sd3drModel  mModel;       //!< Model
    QString     mModelScript; //!< Script which described model. Usually program occupy less space than model
    QMatrix4x4  mTransform;   //!< Transform model to part place
  public:
    Sd3dGraphModel();
    Sd3dGraphModel( const Sd3drModel &model );
    Sd3dGraphModel( const QString &prog );


    Sd3ColorList     bodyColorList() const { return mModel.colorList(); }

    void             bodyColorListSet( const Sd3ColorList &lst ) { mModel.colorListSet( lst ); }

    QString          script() const { return mModelScript; }
    void             scriptSet( const QString script, SdUndo *undo );


    //!
    //! \brief matrixMapInPlace Mapping all faces of model throught given matrix. Operation maked in place
    //! \param matrix           Mapping matrix
    //!
    void             matrixMapInPlace( QMatrix4x4 matrix );

    //!
    //! \brief volumeAdd Append volume of model to result volume
    //! \param volume    Source and result volume
    //!
    virtual void     volumeAdd( QMatrix2x3 &volume ) const override { mModel.volumeAdd( volume ); }

    // SdObject interface
  public:
    virtual QString  getType() const override { return QStringLiteral( SD_TYPE_3D_GRAPH_MODEL ); }

    //!
    //! \brief cloneFrom Overrided function. We copy model from source object
    //! \param src       Source of object from which copy must be made
    //! \param copyMap   Structure for mapping copying substitutes
    //! \param next      Make simple or next copy. Next copy available not for all objects.
    //!                  For example: pin name A23 with next copy return A24
    //!
    virtual void     cloneFrom( const SdObject *src, SdCopyMap &copyMap, bool next ) override;

    //!
    //! \brief json Overloaded function to write object content into json writer
    //!             Overrided function
    //! \param js   Json writer
    //!
    virtual void     json( SdJsonWriter &js ) const override;

    //!
    //! \brief json Overloaded function to read object content from json reader
    //!             Overrided function
    //! \param js   Json reader
    //!
    virtual void     json( const SdJsonReader &js ) override;

    // SdGraph interface
  public:
    virtual SdRect   getOverRect() const override;
    virtual void     draw3d(QOpenGLFunctions_2_0 *f) const override;

  private:
    void             buildModel( const QString &script );
  };

#endif // SD3DGRAPHMODEL_H
