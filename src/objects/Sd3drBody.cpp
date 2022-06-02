#include "Sd3drBody.h"
#include "Sd3dDraw.h"

Sd3drBody::Sd3drBody()
  {

  }



void Sd3drBody::draw(QOpenGLFunctions_2_0 *f, const Sd3dRegion &vertexList, const QMatrix4x4 &map) const
  {
  //Setup color
  mColor.draw( f );

  //Draw all faces
  for( auto face : qAsConst( mFaceList ) ) {
    QVector3D normal = QVector3D::normal( map.map( vertexList.at( face.at(0) ) ), map.map( vertexList.at( face.at(1) ) ), map.map( vertexList.at( face.at(2) ) ) );
    f->glBegin(GL_POLYGON);
    f->glNormal3f( normal.x(), normal.y(), normal.z() );
    for( auto index : qAsConst( face ) ) {
      auto v = map.map( vertexList.at( index ) );
      f->glVertex3f( v.x(), v.y(), v.z() );
      }
    f->glEnd();
    }
  }




void Sd3drBody::json(SdJsonWriter &js) const
  {
  js.jsonListListInt( QStringLiteral("Faces"), mFaceList );
  mColor.json( js );
  }

void Sd3drBody::json(const SdJsonReader &js)
  {
  js.jsonListListInt( QStringLiteral("Faces"), mFaceList );
  mColor.json( js );
  }
