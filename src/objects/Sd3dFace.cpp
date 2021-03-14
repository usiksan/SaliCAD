#include "Sd3dFace.h"





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
