/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  3D body. Body is list of faces with material of all its faces.
  So body has single material for many faces
*/
#include "Sd3drBody.h"
#include "Sd3dDraw.h"
#include "SdJsonIO.h"

Sd3drBody::Sd3drBody()
  {

  }



void Sd3drBody::draw(QOpenGLFunctions_2_0 *f, const Sd3dRegion &vertexList, const QMatrix4x4 &map) const
  {
  //Setup color
  mColor.draw( f );

  //Draw all faces
  for( auto face : qAsConst( mFaceList ) ) {
    if( face.count() < 3 ) continue;
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




//!
//! \brief volumeAdd  Append volume of model to result volume
//! \param volume     Source and result volume
//! \param vertexList Vertex list on which referenced bodies
//! \param map        Map for body transformation
//!
void Sd3drBody::volumeAdd(QMatrix2x3 &volume, const Sd3dRegion &vertexList, const QMatrix4x4 &map) const
  {
  //Draw all faces
  for( auto face : qAsConst( mFaceList ) ) {
    for( auto index : qAsConst( face ) ) {
      auto v = map.map( vertexList.at( index ) );
      if( volume(0,0) > v.x() ) volume(0,0) = v.x();
      if( volume(0,1) < v.x() ) volume(0,1) = v.x();

      if( volume(1,0) > v.y() ) volume(1,0) = v.y();
      if( volume(1,1) < v.y() ) volume(1,1) = v.y();

      if( volume(2,0) > v.z() ) volume(2,0) = v.z();
      if( volume(2,1) < v.z() ) volume(2,1) = v.z();
      }
    }
  }