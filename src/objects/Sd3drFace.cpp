#include "Sd3drFace.h"

Sd3drFace::Sd3drFace()
  {

  }

void Sd3drFace::json(SdJsonWriter &js) const
  {
  js.jsonListInt( QStringLiteral("f"), mVertexRefList );
  }

void Sd3drFace::json(const SdJsonReader &js)
  {
  js.jsonListInt( QStringLiteral("f"), mVertexRefList );
  }



void Sd3drFace::draw(QOpenGLFunctions_2_0 *f, const Sd3dRegion &vertexList, const QMatrix4x4 &map) const
  {
  QVector3D normal = QVector3D::normal( map.map( vertexList.at( mVertexRefList.at(0) ) ), map.map( vertexList.at( mVertexRefList.at(1) ) ), map.map( vertexList.at( mVertexRefList.at(2) ) ) );
  f->glBegin(GL_POLYGON);
  f->glNormal3f( normal.x(), normal.y(), normal.z() );
  for( auto index : qAsConst( mVertexRefList ) ) {
    auto v = map.map( vertexList.at( index ) );
    f->glVertex3f( v.x(), v.y(), v.z() );
    }
  f->glEnd();
  }
