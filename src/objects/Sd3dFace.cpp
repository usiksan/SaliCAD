/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  3D face. Face is flat surface bounded by region
*/
#include "Sd3dFace.h"

#include <QColor>
#include <QDebug>

Sd3dFace::Sd3dFace()
  {

  }




Sd3dFace::Sd3dFace(const Sd3dPointList &region, const Sd3dPointList &normal, const Sd3dFaceMaterial &faceMaterial) :
  mRegion(region),
  mNormal(normal),
  mMaterial(faceMaterial)
  {

  }

Sd3dFace::Sd3dFace(const Sd3dPointList &region, const Sd3dPointList &normal, quint32 faceColor) :
  mRegion(region),
  mNormal(normal),
  mMaterial()
  {
  float r = qRed(faceColor);
  float g = qGreen(faceColor);
  float b = qBlue(faceColor);
  r /= 255.0;
  g /= 255.0;
  b /= 255.0;
  mMaterial.setDiffuseColor( r, g, b );
  }




Sd3dFace::Sd3dFace(const QList<QVector3D> &region, const QList<QVector3D> &normal, const Sd3dFaceMaterial &faceMaterial) :
  mMaterial(faceMaterial)
  {
  for( const auto pt : region )
    mRegion.append( pt );
  for( const auto pt : normal )
    mNormal.append( pt );
  }







//!
//! \brief paint Draw triangle with OpenGL
//! \param f     Pointer to OpenGL functions
//!
void Sd3dFace::paint(QOpenGLFunctions_2_0 *f) const
  {
  //Face
  mMaterial.paint( f );
  f->glBegin(GL_POLYGON);
  for( int i = 0; i < mRegion.count(); i++ ) {
    //Apply normal
    if( i < mNormal.count() )
      mNormal.at(i).normal( f );
    //Append vertex
    mRegion.at(i).vertex( f );
    }
  f->glEnd();
  }





//!
//! \brief write Write triangle to json file
//! \return      JSON object with triangle
//!
QJsonObject Sd3dFace::write() const
  {
  QJsonObject obj;
  mMaterial.write( obj );
  obj.insert( QStringLiteral("region"), sd3dPointListWrite(mRegion) );
  obj.insert( QStringLiteral("normal"), sd3dPointListWrite(mNormal) );
  return obj;
  }




//!
//! \brief read Read triangle from json file
//! \param obj  JSON object with triangle
//!
void Sd3dFace::read(const QJsonObject &obj)
  {
  mMaterial.read( obj );
  sd3dPointListRead( mRegion, obj.value( QStringLiteral("region") ).toArray() );
  sd3dPointListRead( mNormal, obj.value( QStringLiteral("normal") ).toArray() );
  }




//!
//! \brief overRect Return over rect of face projected to XY surface
//! \return         Over rect
//!
SdRect Sd3dFace::overRect() const
  {
  SdRect over;
  for( const auto &pt : mRegion ) {
    over.grow( pt.projectionXY() );
    }
  return over;
  }




