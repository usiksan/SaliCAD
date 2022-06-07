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

#include <QMatrix4x4>
#include <QList>

using Sd3drTransferList = QList<QMatrix4x4>;

class Sd3drInstance
  {
    Sd3drBodyList     mBodyList;
    Sd3drTransferList mCopyList;
  public:
    Sd3drInstance();

    void bodyColorGet( Sd3ColorList &dst ) const;

    void bodyColorSet( const Sd3ColorList &src, int &index );

    void add( const Sd3drBody &ref ) { mBodyList.append( ref ); }

    void addCopy( QMatrix4x4 m ) { mCopyList.append(m); }

    void draw( QOpenGLFunctions_2_0 *f, const Sd3dRegion &vertexList ) const;

    void json( SdJsonWriter &js ) const;

    void json( const SdJsonReader &js );

    //!
    //! \brief volumeAdd  Append volume of model to result volume
    //! \param volume     Source and result volume
    //! \param vertexList Vertex list on which referenced bodies
    //!
    void volumeAdd( QMatrix2x3 &volume, const Sd3dRegion &vertexList ) const;
  };

using Sd3drInstanceList = QList<Sd3drInstance>;

#endif // SD3DRINSTANCE_H
