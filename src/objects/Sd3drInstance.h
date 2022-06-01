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

    void add( const Sd3drBody &ref ) { mBodyList.append( ref ); }

    void addCopy( QMatrix4x4 m ) { mCopyList.append(m); }

    void draw( QOpenGLFunctions_2_0 *f, const Sd3dRegion &vertexList ) const;

    void json( SdJsonWriter &js ) const;

    void json( const SdJsonReader &js );
  };

using Sd3drInstanceList = QList<Sd3drInstance>;

#endif // SD3DRINSTANCE_H
