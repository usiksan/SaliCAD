#include "Sd3drInstance.h"

Sd3drInstance::Sd3drInstance()
  {

  }



void Sd3drInstance::draw(QOpenGLFunctions_2_0 *f, const Sd3drBodyList &bodyList, const Sd3drFaceList &faceList, const Sd3dRegion &vertexList) const
  {
  for( auto const &t : qAsConst( mCopyList ) ) {
    QMatrix4x4 map = t.mapGet();
    for( auto index : qAsConst( mBodyRefList ) )
      bodyList.at(index).draw( f, faceList, vertexList, map );
    }
  }
