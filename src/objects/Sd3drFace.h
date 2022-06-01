#ifndef SD3DRFACE_H
#define SD3DRFACE_H

#include "Sd3dRegion.h"
#include "SdJsonIO.h"

#include <QOpenGLFunctions_2_0>
#include <QList>
#include <QMatrix4x4>

class Sd3drFace
  {
    QList<int> mVertexRefList;
  public:
    Sd3drFace() {}

    Sd3drFace( QList<int> refList ) : mVertexRefList(refList) {}

    void       addRef( int ref ) { mVertexRefList.append( ref ); }

    QList<int> vertexRefList() const { return mVertexRefList; }

    void       json( SdJsonWriter &js ) const;

    void       json( const SdJsonReader &js );

    void       draw( QOpenGLFunctions_2_0 *f, const Sd3dRegion &vertexList, const QMatrix4x4 &map ) const;
  };


using Sd3drFaceList = QList<Sd3drFace>;

#endif // SD3DRFACE_H
