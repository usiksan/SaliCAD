#include "Sd3dFaceSet.h"
#include "SdJsonIO.h"

Sd3dFaceSet::Sd3dFaceSet()
  {

  }




//!
//! \brief faceAdd Appends face to face list of model
//! \param face    Appended face
//!
void Sd3dFaceSet::faceAdd(const Sd3dFaceEx &face)
  {
  mFaceList.append( face );
  }





//!
//! \brief json Overloaded function to write object content into json writer
//!             Overrided function
//! \param js   Json writer
//!
void Sd3dFaceSet::json(SdJsonWriter &js) const
  {
  js.jsonList( js, QStringLiteral("faces"), mFaceList );
  Sd3dGraph::json( js );
  }





//!
//! \brief json Overloaded function to read object content from json reader
//!             Overrided function
//! \param js   Json reader
//!
void Sd3dFaceSet::json(const SdJsonReader &js)
  {
  js.jsonList( js, QStringLiteral("faces"), mFaceList );
  Sd3dGraph::json( js );
  }








SdRect Sd3dFaceSet::getOverRect() const
  {
  if( mFaceList.count() ) {
    //At least one face present
    SdRect over = mFaceList.first().overRect();
    for( const auto &face : mFaceList )
      over.grow( face.overRect() );
    return over;
    }
  return SdRect{};
  }





void Sd3dFaceSet::draw3d(QOpenGLFunctions_2_0 *f) const
  {
  for( const auto &face : mFaceList )
    face.paint( f );
  }
