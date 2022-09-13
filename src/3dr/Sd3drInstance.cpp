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

void Sd3drInstance::clear()
  {
  mBodyList.clear();
  mCopyList.clear();
  }

void Sd3drInstance::bodyColorGet(Sd3ColorList &dst) const
  {
  for( auto const &body : qAsConst( mBodyList ) )
    body.colorListGet( dst );
  }

void Sd3drInstance::bodyColorSet(const Sd3ColorList &src, int &index)
  {
  for( int i = 0; i < mBodyList.count(); i++ )
    mBodyList[i].colorListSet( src.at(index++) );
  }



void Sd3drInstance::draw(QOpenGLFunctions_2_0 *f, const Sd3drRegion &vertexList) const
  {
  for( auto const &map : qAsConst( mCopyList ) ) {
    for( auto const &body : qAsConst( mBodyList ) )
      body.draw( f, vertexList, map );
    }
  }

void Sd3drInstance::json(SvJsonWriter3d &js) const
  {
  js.jsonList( js, QStringLiteral("Bodies"), mBodyList );
  js.jsonListMatrix4x4( QStringLiteral("Copies"), mCopyList );
  }

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
