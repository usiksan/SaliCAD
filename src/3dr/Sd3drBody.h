/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  3D body. Body is list of faces with material of all its faces.
  So body has single material for many faces
*/
#ifndef SD3DRBODY_H
#define SD3DRBODY_H

#include "Sd3drRegion.h"
#include "Sd3drFace.h"
#include "Sd3drMaterial.h"

#include <QList>
#include <QColor>


class Sd3drBody
  {
    Sd3drFaceList  mFaceList; //!< List of faces. Each face is list of vertex of polygon in global vertex list of model
    Sd3drMaterial  mColor;    //!< Material of all faces of this body
  public:
    Sd3drBody();

    //!
    //! \brief isEmpty Return true if face list is empty, color may be any color
    //! \return        true if face list is empty
    //!
    bool isEmpty() const { return mFaceList.isEmpty(); }

    //!
    //! \brief clear Clear face list of body
    //!
    void clear() { mFaceList.clear(); }

    //!
    //! \brief colorListGet Retrive color to color list
    //! \param dst          Destignation color list
    //!
    void colorListGet( Sd3ColorList &dst ) const { dst.append( mColor ); }

    //!
    //! \brief colorListSet Setup new color
    //! \param src          New color
    //!
    void colorListSet( const Sd3drMaterial &src ) { mColor = src; }

    //!
    //! \brief colorSet Sets ambient and diffuse colors of body to specified color
    //! \param c        Color for ambient and diffuse
    //!
    void colorSet( QColor c ) { mColor.mAmbient = mColor.mDiffuse = c; }

    //!
    //! \brief faceAppend Appends face to face list of body
    //! \param ref        Face to append
    //!
    void faceAppend( const Sd3drFace &ref ) { mFaceList.append( ref ); }

    //!
    //! \brief faceAppend Append face list to face list of body
    //! \param list       Face list to append
    //!
    void faceAppend( const Sd3drFaceList &list ) { mFaceList.append(list); }

    //!
    //! \brief draw       Draw body in 3d space
    //! \param f          OpenGL functions
    //! \param vertexList Model vertex list because body contains only indexes of vertexes in this list
    //! \param map        Conversion map
    //!
    void draw( QOpenGLFunctions_2_0 *f, const Sd3drRegion &vertexList, const QMatrix4x4 &map ) const;

    //!
    //! \brief json Perform writing to json
    //! \param js   JSON writer
    //!
    void json( SvJsonWriter &js ) const;

    //!
    //! \brief json Perform reading from json
    //! \param js   JSON reader
    //!
    void json( const SvJsonReader &js );

    //!
    //! \brief volumeAdd  Append volume of model to result volume
    //! \param volume     Source and result volume
    //! \param vertexList Vertex list on which referenced bodies
    //! \param map        Map for body transformation
    //!
    void volumeAdd( QMatrix2x3 &volume, const Sd3drRegion &vertexList, const QMatrix4x4 &map ) const;
  };


using Sd3drBodyList = QList<Sd3drBody>;

#endif // SD3DRBODY_H
