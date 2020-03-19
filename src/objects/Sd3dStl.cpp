#include "Sd3dStl.h"

#include <QFile>
#include <QDebug>

Sd3dStl::Sd3dStl() :
  Sd3dObject()
  {

  }



//!
//! \brief importStl Read model from STL file
//! \param file      STL file
//! \result          true if import successfull or false in other case
//!
bool Sd3dStl::importStl(QIODevice *file)
  {
  //Skeep header
  file->seek( 80 );

  //Read triangle count
  qint32 triangleCount = 0;
  file->read( static_cast<char*>( static_cast<void*>((&triangleCount)) ), 4 );
  //qDebug() << "triangle count" << triangleCount;

  Sd3dTriangle triangle;
  for( int i = 0; i < triangleCount; i++ ) {
    //Read triangle
    triangle.readStl( file );
    //Append it to list
    mTriangleList.append( triangle );
    }

  return true;
  }




//!
//! \brief importStlFromFile Read model from STL file which represented by its path
//! \param fname             Full path to STL file
//! \return                  Pointer to Sd3dStl object if import was successfull or nullptr in other case
//!
Sd3dStl *Sd3dStl::importStlFromFile(QString fname)
  {
  QFile file(fname);

  if( file.open( QIODevice::ReadOnly ) ) {
    Sd3dStl *stl = new Sd3dStl();
    if( stl->importStl( &file ) )
      return stl;
    delete stl;
    }

  return nullptr;
  }







void Sd3dStl::writeObject(QJsonObject &obj) const
  {
  Sd3dObject::writeObject( obj );
  QJsonArray ar;
  for( const auto &triangle : mTriangleList ) {
    ar.append( triangle.write() );
    }
  obj.insert( QStringLiteral("faces"), ar );
  }





void Sd3dStl::readObject(SdObjectMap *map, const QJsonObject obj)
  {
  Sd3dObject::readObject( map, obj );
  QJsonArray ar = obj.value( QStringLiteral("faces") ).toArray();
  mTriangleList.clear();
  Sd3dTriangle triangle;
  for( const auto value : ar ) {
    triangle.read( value.toObject() );
    mTriangleList.append( triangle );
    }
  }




SdRect Sd3dStl::getOverRect() const
  {
  //TODO over rect for 3d model
  return SdRect{};
  }




void Sd3dStl::draw3d(QOpenGLFunctions_2_0 *f) const
  {
  for( const Sd3dTriangle &triangle : mTriangleList )
    triangle.paint( f );
  }
