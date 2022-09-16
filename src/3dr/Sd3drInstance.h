/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  3D instance of body. Instance consist of one or more bodies and number of matrix for its copy
  Body is list of faces with material of all its faces.
*/
#ifndef SD3DRINSTANCE_H
#define SD3DRINSTANCE_H

#include "Sd3drBody.h"
#include "SvLib/SvJson3dIO.h"

#include <QMatrix4x4>
#include <QList>

using Sd3drTransferList = QList<QMatrix4x4>;

class Sd3drInstance
  {
    Sd3drBodyList     mBodyList; //!< Body list of instance. Each body contains face list and associated color
    Sd3drTransferList mCopyList; //!< This list contains matrixes. So, instance contains so mutch copies of body list as this list contains
  public:
    Sd3drInstance();

    //!
    //! \brief isEmpty Instance is empty when no body in body list
    //! \return        true when no body in body list
    //!
    bool isEmpty() const { return mBodyList.isEmpty(); }

    //!
    //! \brief isNoCopy Returns mCopyList is empty
    //! \return         true when copy list is empty
    //!
    bool isNoCopy() const { return mCopyList.isEmpty(); }

    //!
    //! \brief clear Clear both list: body list and copy list
    //!
    void clear();

    //!
    //! \brief bodyColorGet Collect colors for each body
    //! \param dst          Destignation color list
    //!
    void bodyColorGet( Sd3ColorList &dst ) const;

    //!
    //! \brief bodyColorSet Sets color for each body
    //! \param src          Source list of colors
    //! \param index        On input contains start index for instance bodies colors, on exit - contains end index for instance bodies colors
    //!
    void bodyColorSet( const Sd3ColorList &src, int &index );

    //!
    //! \brief add Adds one body to body list. Each body contains face list with associated color
    //! \param ref Body for append
    //!
    void add( const Sd3drBody &ref ) { mBodyList.append( ref ); }

    //!
    //! \brief addCopy Adds one copy to copy list
    //! \param m       Matrix which define transformation for appended copy
    //!
    void addCopy( QMatrix4x4 m ) { mCopyList.append(m); }

    //!
    //! \brief draw       Draw instance in 3d space
    //! \param f          OpenGL functions
    //! \param vertexList Vertex list because instance contains body list which consist only indexes of vertex
    //! \param m          Conversion for instance
    //!
    void draw( QOpenGLFunctions_2_0 *f, const Sd3drRegion &vertexList, QMatrix4x4 m ) const;

    //!
    //! \brief json Perform writing to json
    //! \param js   JSON writer
    //!
    void json( SvJsonWriter3d &js ) const;

    //!
    //! \brief json Perform reading from json
    //! \param js   JSON reader
    //!
    void json( const SvJsonReader3d &js );

    //!
    //! \brief volumeAdd  Append volume of model to result volume
    //! \param volume     Source and result volume
    //! \param vertexList Vertex list on which referenced bodies
    //!
    void volumeAdd( QMatrix2x3 &volume, const Sd3drRegion &vertexList ) const;
  };

using Sd3drInstanceList = QList<Sd3drInstance>;

#endif // SD3DRINSTANCE_H
