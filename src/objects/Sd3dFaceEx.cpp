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
#include "Sd3dFaceEx.h"

#include <QColor>
#include <QDebug>

Sd3dFaceEx::Sd3dFaceEx()
  {

  }




Sd3dFaceEx::Sd3dFaceEx(const Sd3dPointList &region, const Sd3dPointList &normal, const Sd3dFaceMaterial &faceMaterial) :
  mRegion(region),
  mNormal(normal),
  mMaterial(faceMaterial)
  {

  }

Sd3dFaceEx::Sd3dFaceEx(const Sd3dPointList &region, const Sd3dPointList &normal, quint32 faceColor) :
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




Sd3dFaceEx::Sd3dFaceEx(const QList<QVector3D> &region, const QList<QVector3D> &normal, const Sd3dFaceMaterial &faceMaterial) :
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
void Sd3dFaceEx::paint(QOpenGLFunctions_2_0 *f) const
  {
  //Face
  f->glBegin(GL_POLYGON);
  mMaterial.paint( f );
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
//! \brief json Overloaded function to write object content into json writer
//! \param js   Json writer
//!
void Sd3dFaceEx::json(SvJsonWriter &js) const
  {
  mMaterial.json( js );
  json3dPointList( js, QStringLiteral("region"), mRegion );
  json3dPointList( js, QStringLiteral("normal"), mNormal );
  }




//!
//! \brief json Overloaded function to read object content from json reader
//! \param js   Json reader
//!
void Sd3dFaceEx::json(const SvJsonReader &js)
  {
  mMaterial.json( js );
  json3dPointList( js, QStringLiteral("region"), mRegion );
  json3dPointList( js, QStringLiteral("normal"), mNormal );
  }







//!
//! \brief overRect Return over rect of face projected to XY surface
//! \return         Over rect
//!
SdRect Sd3dFaceEx::overRect() const
  {
  SdRect over;
  for( const auto &pt : mRegion ) {
    over.grow( pt.projectionXY() );
    }
  return over;
  }




