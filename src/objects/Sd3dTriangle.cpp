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
  //Setup triangle color
  f->glColor3b( qRed(mColor) >> 1, qGreen(mColor) >> 1, qBlue(mColor) >> 1 );
  //Draw triangle
  f->glBegin(GL_POLYGON);
  f->glNormal3d( mNormal.x(), mNormal.y(), mNormal.z() );
  f->glVertex3d( mA.x(), mA.y(), mA.z() );
  f->glVertex3d( mB.x(), mB.y(), mB.z() );
  f->glVertex3d( mC.x(), mC.y(), mC.z() );
  f->glEnd();
  }





//!
//! \brief write Write triangle to json file
//! \return      JSON object with triangle
//!
QJsonObject Sd3dTriangle::write() const
  {
  QJsonObject obj;
  mA.write( QStringLiteral("a"), obj );
  mB.write( QStringLiteral("b"), obj );
  mC.write( QStringLiteral("c"), obj );
  mNormal.write( QStringLiteral("normal"), obj );
  obj.insert( QStringLiteral("color"), static_cast<int>(mColor) );
  return obj;
  }







//!
//! \brief read Read triangle from json file
//! \param obj  JSON object with triangle
//!
void Sd3dTriangle::read(const QJsonObject &obj)
  {
  mA.read( QStringLiteral("a"), obj );
  mB.read( QStringLiteral("b"), obj );
  mC.read( QStringLiteral("c"), obj );
  mNormal.read( QStringLiteral("normal"), obj );
  mColor = obj.value( QStringLiteral("color") ).toInt();
  }






//!
//! \brief readStlVector3d Read 3d vector from STL file (3 values)
//! \param file            STL file
//! \return                Readed vector converted to mcm
//!
Sd3dPoint Sd3dTriangle::readStlVector3d(QIODevice *file)
  {
  float x = readStlFloat( file );
  float y = readStlFloat( file );
  float z = readStlFloat( file );
  return Sd3dPoint( x, y, z );
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
