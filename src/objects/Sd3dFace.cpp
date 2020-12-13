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

  }




//!
//! \brief read Read triangle from json file
//! \param obj  JSON object with triangle
//!
void Sd3dFace::read(const QJsonObject &obj)
  {

  }




