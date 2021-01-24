#include "Sd3dFaceSet.h"

Sd3dFaceSet::Sd3dFaceSet()
  {

  }




//!
//! \brief faceAdd Appends face to face list of model
//! \param face    Appended face
//!
void Sd3dFaceSet::faceAdd(const Sd3dFace &face)
  {
  mFaceList.append( face );
  }




void Sd3dFaceSet::writeObject(QJsonObject &obj) const
  {
  Sd3dObject::writeObject( obj );
  QJsonArray ar;
  for( const auto &face : mFaceList ) {
    ar.append( face.write() );
    }
  obj.insert( QStringLiteral("faces"), ar );
  }





void Sd3dFaceSet::readObject(SdObjectMap *map, const QJsonObject obj)
  {
  Sd3dObject::readObject( map, obj );
  QJsonArray ar = obj.value( QStringLiteral("faces") ).toArray();
  mFaceList.clear();
  Sd3dFace face;
  for( const auto value : ar ) {
    face.read( value.toObject() );
    mFaceList.append( face );
    }
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
