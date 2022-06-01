#include "Sd3drInstance.h"

Sd3drInstance::Sd3drInstance()
  {

  }



void Sd3drInstance::draw(QOpenGLFunctions_2_0 *f, const Sd3dRegion &vertexList) const
  {
  for( auto const &map : qAsConst( mCopyList ) ) {
    for( auto const &body : qAsConst( mBodyList ) )
      body.draw( f, vertexList, map );
    }
  }
