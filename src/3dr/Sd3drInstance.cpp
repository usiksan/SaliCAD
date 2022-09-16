/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  3D instance of body. Instance consist of one or more bodies and number of matrix for its copy
  Body is list of faces with material of all its faces.
*/
#include "Sd3drInstance.h"


Sd3drInstance::Sd3drInstance()
  {

  }




//!
//! \brief clear Clear both list: body list and copy list
//!
void Sd3drInstance::clear()
  {
  mBodyList.clear();
  mCopyList.clear();
  }




//!
//! \brief bodyColorGet Collect colors for each body
//! \param dst          Destignation color list
//!
void Sd3drInstance::bodyColorGet(Sd3ColorList &dst) const
  {
  for( auto const &body : qAsConst( mBodyList ) )
    body.colorListGet( dst );
  }




//!
//! \brief bodyColorSet Sets color for each body
//! \param src          Source list of colors
//! \param index        On input contains start index for instance bodies colors, on exit - contains end index for instance bodies colors
//!
void Sd3drInstance::bodyColorSet(const Sd3ColorList &src, int &index)
  {
  for( int i = 0; i < mBodyList.count(); i++ )
    mBodyList[i].colorListSet( src.at(index++) );
  }



//!
//! \brief draw       Draw instance in 3d space
//! \param f          OpenGL functions
//! \param vertexList Vertex list because instance contains body list which consist only indexes of vertex
//! \param m          Conversion for instance
//!
void Sd3drInstance::draw(QOpenGLFunctions_2_0 *f, const Sd3drRegion &vertexList, QMatrix4x4 m) const
  {
  for( auto const &map : qAsConst( mCopyList ) ) {
    //Build matrix for conversion
    QMatrix4x4 mc = m * map;
    for( auto const &body : qAsConst( mBodyList ) )
      body.draw( f, vertexList, mc );
    }
  }




//!
//! \brief json Perform writing to json
//! \param js   JSON writer
//!
void Sd3drInstance::json(SvJsonWriter3d &js) const
  {
  js.jsonList( js, QStringLiteral("Bodies"), mBodyList );
  js.jsonListMatrix4x4( QStringLiteral("Copies"), mCopyList );
  }




//!
//! \brief json Perform reading from json
//! \param js   JSON reader
//!
void Sd3drInstance::json(const SvJsonReader3d &js)
  {
  js.jsonList( js, QStringLiteral("Bodies"), mBodyList );
  js.jsonListMatrix4x4( QStringLiteral("Copies"), mCopyList );
  }



//!
//! \brief volumeAdd  Append volume of model to result volume
//! \param volume     Source and result volume
//! \param vertexList Vertex list on which referenced bodies
//!
void Sd3drInstance::volumeAdd(QMatrix2x3 &volume, const Sd3drRegion &vertexList) const
  {
  for( auto const &map : qAsConst( mCopyList ) ) {
    for( auto const &body : qAsConst( mBodyList ) )
      body.volumeAdd( volume, vertexList, map );
    }
  }
