/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  3d object reader for stl files
*/
#include "Sd3dReaderStl.h"

#include <QFile>
#include <QDebug>






//!
//! \brief readStlFloat Read standard IEEE 32bit float from STL file
//! \param file         STL file
//! \return             Readed float
//!
static float readStlFloat(QIODevice *file)
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
static quint16 readStlUShort(QIODevice *file)
  {
  quint16 val;
  file->read( static_cast<char*>( static_cast<void*>(&val)), 2 );
  return val;
  }



//!
//! \brief readStlVector3d Read 3d vector from STL file (3 values)
//! \param file            STL file
//! \return                Readed vector converted to mcm
//!
Sd3dPoint readStlVector3d(QIODevice *file)
  {
  float x = readStlFloat( file );
  float y = readStlFloat( file );
  float z = readStlFloat( file );
  return Sd3dPoint( x, y, z );
  }




//!
//! \brief importStlFromFile Read model from STL file which represented by its path
//! \param fname             Full path to STL file
//! \return                  Pointer to Sd3dFaceSet object if import was successfull or nullptr in other case
//!
Sd3dFaceSet *Sd3dReaderStl::importStlFromFile(QString fname)
  {
  QFile file(fname);

  if( file.open( QIODevice::ReadOnly ) ) {
    Sd3dFaceSet *stl = new Sd3dFaceSet();

    //Skeep header
    file.seek( 80 );

    //Read triangle count
    qint32 triangleCount = 0;
    file.read( static_cast<char*>( static_cast<void*>((&triangleCount)) ), 4 );
    //qDebug() << "triangle count" << triangleCount;

    for( int i = 0; i < triangleCount; i++ ) {
      //Read triangle
      //Read normal vector
      Sd3dPoint normal = readStlVector3d( &file );
      //Read triangle vertexes
      Sd3dPoint vertexA = readStlVector3d( &file );
      Sd3dPoint vertexB = readStlVector3d( &file );
      Sd3dPoint vertexC = readStlVector3d( &file );
      //Read triangle attributes which we treat as color
      quint16 srcColor = readStlUShort( &file );
      quint32 faceColor;
      if( srcColor & 0x8000 ) {
        //Color present
        faceColor = qRgba( ((srcColor >> 7) & 0xf8), (srcColor >> 2) & 0xf8, (srcColor << 3) & 0xf8, 0xff );
        }
      else {
        //No color defined, setup gray color
        faceColor = qRgba( 30, 30, 30, 0xff );
        }
      Sd3dFace triangle( {vertexA, vertexB, vertexC}, normal, faceColor );
      //Append it to list
      stl->faceAdd( triangle );
      }

    return stl;
    }
  return nullptr;
  }
