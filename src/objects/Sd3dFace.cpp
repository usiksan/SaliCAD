#include "Sd3dFace.h"



//!
//! \brief Sd3dFace Builds triangle face with three vertex's and face color
//! \param p[0,1,2] Vertexies
//! \param color    Face color
//!
Sd3dFace::Sd3dFace(QVector3D p0, QVector3D p1, QVector3D p2, QColor color) :
  mContour( { p0, p1, p2 } ),
  mColor( color )
  {
  }



//!
//! \brief json Overloaded function to write object content into json writer
//! \param js   Json writer
//!
void Sd3dFace::json(SvJsonWriter &js) const
  {
  json3dRegion( js, QStringLiteral("contour"), mContour );
  js.jsonColor( QStringLiteral("color"), mColor );
  }





//!
//! \brief json Overloaded function to read object content from json reader
//! \param js   Json reader
//!
void Sd3dFace::json(const SvJsonReader &js)
  {
  json3dRegion( js, QStringLiteral("contour"), mContour );
  js.jsonColor( QStringLiteral("color"), mColor );
  }






//!
//! \brief volume Append face to volume
//! \param vol    Volume source and result
//!
void Sd3dFace::volume(QMatrix2x3 &vol) const
  {
  for( auto const &v : mContour ) {
    if( vol(0,0) > v.x() ) vol(0,0) = v.x();
    if( vol(0,1) < v.x() ) vol(0,1) = v.x();

    if( vol(1,0) > v.y() ) vol(1,0) = v.y();
    if( vol(1,1) < v.y() ) vol(1,1) = v.y();

    if( vol(2,0) > v.z() ) vol(2,0) = v.z();
    if( vol(2,1) < v.z() ) vol(2,1) = v.z();

    }
  }




