#include "Sd3drInstance.h"

Sd3drInstance::Sd3drInstance()
  {

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



void Sd3drInstance::draw(QOpenGLFunctions_2_0 *f, const Sd3dRegion &vertexList) const
  {
  for( auto const &map : qAsConst( mCopyList ) ) {
    for( auto const &body : qAsConst( mBodyList ) )
      body.draw( f, vertexList, map );
    }
  }

void Sd3drInstance::json(SdJsonWriter &js) const
  {
  js.jsonList( js, QStringLiteral("Bodies"), mBodyList );
  js.jsonListMatrix4x4( QStringLiteral("Copies"), mCopyList );
  }

void Sd3drInstance::json(const SdJsonReader &js)
  {
  js.jsonList( js, QStringLiteral("Bodies"), mBodyList );
  js.jsonListMatrix4x4( QStringLiteral("Copies"), mCopyList );
  }
