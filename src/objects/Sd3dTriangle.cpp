#include "Sd3dTriangle.h"

#include <QIODevice>
#include <QRgb>

Sd3dTriangle::Sd3dTriangle()
  {

  }





//!
//! \brief readStl Read triangle STL description from file
//! \param file    File from which need to read triangle description
//!
void Sd3dTriangle::readStl(QIODevice *file)
  {
  //Read normal vector
  mNormal = readStlVector3d( file );
  //Read triangle vertexes
  mA = readStlVector3d( file );
  mB = readStlVector3d( file );
  mC = readStlVector3d( file );
  //Read triangle attributes which we treat as color
  quint16 color = readStlUShort( file );
  if( color & 0x8000 ) {
    //Color present
    mColor = qRgba( ((color >> 7) & 0xf8), (color >> 2) & 0xf8, (color << 3) & 0xf8, 0xff );
    }
  else {
    //No color defined, setup gray color
    mColor = qRgba( 30, 30, 30, 0xff );
    }
  }





//!
//! \brief paint Draw triangle with OpenGL
//! \param f     Pointer to OpenGL functions
//!
void Sd3dTriangle::paint(QOpenGLFunctions_2_0 *f) const
  {
  f->glBegin(GL_POLYGON);
  f->glNormal3d( mNormal.x(), mNormal.y(), mNormal.z() );
  f->glVertex3d( mA.x(), mA.y(), mA.z() );
  f->glVertex3d( mB.x(), mB.y(), mB.z() );
  f->glVertex3d( mC.x(), mC.y(), mC.z() );
  f->glEnd();
  }






//!
//! \brief readStlFloat Read standard IEEE 32bit float from STL file
//! \param file         STL file
//! \return             Readed float
//!
float Sd3dTriangle::readStlFloat(QIODevice *file)
  {
  float val;
  file->read( static_cast<char*>( static_cast<void*>(&val)), 4 );
  //Convert to mcm
  return val * 1000.0;
  }





//!
//! \brief readStlUShort Read unsigned short from STL file
//! \param file          STL file
//! \return              Readed unsigned short
//!
quint16 Sd3dTriangle::readStlUShort(QIODevice *file)
  {
  quint16 val;
  file->read( static_cast<char*>( static_cast<void*>(&val)), 2 );
  return val;
  }
