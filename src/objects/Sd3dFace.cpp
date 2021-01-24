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




Sd3dFace::Sd3dFace(QList<Sd3dPoint> region, Sd3dPoint normal, quint32 faceColor) :
  mRegion(region),
  mNormal(normal),
  mFaceColor(faceColor)
  {

  }







//!
//! \brief paint Draw triangle with OpenGL
//! \param f     Pointer to OpenGL functions
//!
void Sd3dFace::paint(QOpenGLFunctions_2_0 *f) const
  {
  //Face
  f->glColor3b( qRed(mFaceColor) >> 1, qGreen(mFaceColor) >> 1, qBlue(mFaceColor) >> 1 );
  f->glBegin(GL_POLYGON);
  f->glNormal3f( mNormal.fxmm(), mNormal.fymm(), mNormal.fzmm() );
  for( const Sd3dPoint &p : mRegion ) {
    //qDebug() << "Sd3dFace::paint" << p.fxmm() << p.fymm() << p.fzmm();
    //f->glVertex3f( p.fxmm(), p.fymm(), p.fzmm() );
    p.vertex( f );
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
  obj.insert( QStringLiteral("color"), static_cast<int>(mFaceColor) );
  mNormal.write( QStringLiteral("normal"), obj );
  QJsonArray region;
  for( auto const &pt : mRegion ) {
    region.append( pt.write() );
    }
  obj.insert( QStringLiteral("region"), region );
  return obj;
  }




//!
//! \brief read Read triangle from json file
//! \param obj  JSON object with triangle
//!
void Sd3dFace::read(const QJsonObject &obj)
  {
  mFaceColor = static_cast<quint32>( obj.value( QStringLiteral("color") ).toInt() );
  mNormal.read( QStringLiteral("normal"), obj );
  mRegion.clear();
  QJsonArray region = obj.value( QStringLiteral("region") ).toArray();
  Sd3dPoint pt;
  for( auto it = region.cbegin(); it != region.cend(); it++ ) {
    pt.read( it->toObject() );
    mRegion.append( pt );
    }
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




