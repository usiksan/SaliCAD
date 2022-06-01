#include "Sd3drBody.h"
#include "Sd3dDraw.h"

Sd3drBody::Sd3drBody()
  {

  }

void Sd3drBody::draw(QOpenGLFunctions_2_0 *f, const Sd3drFaceList &faceList, const Sd3dRegion &vertexList, const QMatrix4x4 &map) const
  {
  //Setup color
  float fcolor[4];
  Sd3dDraw::colorToFloat( mColorAmbient, fcolor );
  f->glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT, fcolor );

  Sd3dDraw::colorToFloat( mColorDiffuse, fcolor );
  f->glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE, fcolor );

  Sd3dDraw::colorToFloat( mColorSpecular, fcolor );
  f->glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, fcolor );

  //Draw all faces
  for( auto index : qAsConst( mFaceRefList ) ) {
    faceList.at(index).draw( f, vertexList, map );
    }
  }
