#ifndef SD3DRINSTANCE_H
#define SD3DRINSTANCE_H

#include "Sd3drBody.h"
#include "Sd3drTransfer.h"

class Sd3drInstance
  {
    QList<int>           mBodyRefList;
    QList<Sd3drTransfer> mCopyList;
  public:
    Sd3drInstance();

    void addRef( int ref ) { mBodyRefList.append( ref ); }

    void addCopy( Sd3drTransfer t ) { mCopyList.append(t); }

    void draw( QOpenGLFunctions_2_0 *f, const Sd3drBodyList &bodyList, const Sd3drFaceList &faceList, const Sd3dRegion &vertexList ) const;

    void json( SdJsonWriter &js ) const;

    void json( const SdJsonReader &js );
  };

using Sd3drInstanceList = QList<Sd3drInstance>;

#endif // SD3DRINSTANCE_H
