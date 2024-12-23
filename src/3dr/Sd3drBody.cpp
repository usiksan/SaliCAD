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
#include "SvLib/SvJson3dIO.h"

Sd3drBody::Sd3drBody()
  {

  }



//!
//! \brief draw       Draw body in 3d space
//! \param f          OpenGL functions
//! \param vertexList Model vertex list because body contains only indexes of vertexes in this list
//! \param map        Conversion map
//!
void Sd3drBody::draw(QOpenGLFunctions_2_0 *f, const Sd3drRegion &vertexList, const QMatrix4x4 &map) const
  {
  //Setup color
  mColor.draw( f );

  //Draw all faces
  for( auto face : std::as_const( mFaceList ) ) {
    if( face.count() < 3 ) continue;
    QVector3D normal = QVector3D::normal( map.map( vertexList.at( face.at(0) ) ), map.map( vertexList.at( face.at(1) ) ), map.map( vertexList.at( face.at(2) ) ) );
    f->glBegin(GL_POLYGON);
    f->glNormal3f( normal.x(), normal.y(), normal.z() );
    for( auto index : std::as_const( face ) ) {
      auto v = map.map( vertexList.at( index ) );
      f->glVertex3f( v.x(), v.y(), v.z() );
      }
    f->glEnd();
    }
  }




//!
//! \brief json Perform writing to json
//! \param js   JSON writer
//!
void Sd3drBody::json(SvJsonWriter &js) const
  {
  js.jsonListListInt( QStringLiteral("Faces"), mFaceList );
  mColor.json( js );
  }




//!
//! \brief json Perform reading from json
//! \param js   JSON reader
//!
void Sd3drBody::json(const SvJsonReader &js)
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
void Sd3drBody::volumeAdd(QMatrix2x3 &volume, const Sd3drRegion &vertexList, const QMatrix4x4 &map) const
  {
  //Draw all faces
  for( auto face : std::as_const( mFaceList ) ) {
    for( auto index : std::as_const( face ) ) {
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
