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
//! \brief write Writes face to JSON object
//! \return      JSON object with face
//!
QJsonObject Sd3dFace::write() const
  {
  QJsonObject obj;
  obj.insert( QStringLiteral("contour"), sd3dRegionWrite(mContour) );
  obj.insert( QStringLiteral("color"), mColor.name() );
  return obj;
  }




//!
//! \brief read Read face from JSON object
//! \param obj  JSON object with face
//!
void Sd3dFace::read(const QJsonObject &obj)
  {
  mContour = sd3dRegionRead( obj.value(QStringLiteral("contour")).toArray() );
  mColor   = QColor( obj.value(QStringLiteral("color")).toString() );
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




