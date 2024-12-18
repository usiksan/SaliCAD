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
#include "SdConfig.h"
#include "Sd3dReaderStl.h"
#include "objects/Sd3dPoint.h"

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
  return val;
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
Sd3dGraphModel *Sd3dReaderStl::importStlFromFile(QString fname)
  {
  QFile file(fname);

  if( file.open( QIODevice::ReadOnly ) ) {
    Sd3drModel model;
    Sd3drFaceList faceList;
    Sd3drInstance inst;
    inst.addCopy( QMatrix4x4() );

    //Skeep header
    file.seek( 80 );

    //Read triangle count
    qint32 triangleCount = 0;
    file.read( static_cast<char*>( static_cast<void*>((&triangleCount)) ), 4 );
    //qDebug() << "triangle count" << triangleCount;

    quint32 currentColor = qRgba( 30, 30, 30, 0xff );
    for( int i = 0; i < triangleCount; i++ ) {
      //Read triangle
      //Read normal vector, we don't use normal vector
      readStlVector3d( &file );
      //Read triangle vertexes
      int vertexA = model.vertexAppend( readStlVector3d( &file ) );
      int vertexB = model.vertexAppend( readStlVector3d( &file ) );
      int vertexC = model.vertexAppend( readStlVector3d( &file ) );
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
      if( faceColor != currentColor && faceList.count() ) {
        Sd3drBody body;
        body.faceAppend( faceList );
        body.colorSet( currentColor );
        inst.add( body );
        faceList.clear();
        }
      currentColor = faceColor;
      Sd3drFace face( {vertexA, vertexB, vertexC} );
      //Append it to list
      faceList.append( face );
      }

    if( faceList.count() ) {
      Sd3drBody body;
      body.faceAppend( faceList );
      body.colorSet( currentColor );
      inst.add( body );
      }

    model.instanceAppend( inst );

    return new Sd3dGraphModel( model );
    }
  return nullptr;
  }
