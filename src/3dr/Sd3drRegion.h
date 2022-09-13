/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Region of 3D point vertexies
  Assume coords in mm
*/
#ifndef SD3DRREGION_H
#define SD3DRREGION_H

#include "SvLib/SvJsonIO.h"

#include <QVector3D>
#include <QList>
#include <QJsonArray>
#include <QMatrix4x4>

using Sd3drRegion = QList<QVector3D>; //! Region of 3d points


inline double intMcmToFloatMm( int mcm )
  {
  //Convert to double
  double d = mcm;
  //Convert mcm to meter
  return d / 1000.0;
  }

inline int floatMmToIntMcm( float mm )
  {
  return mm * 1000.0;
  }

void json3dRegion( SvJsonWriter &js, const QString &key, const Sd3drRegion &region );
void json3dRegion( const SvJsonReader &js, const QString &key, Sd3drRegion &region );

//!
//! \brief sd3dRegionWrite Write region of 3d points into JSON array object
//! \param region          Region
//! \return                JSON array object
//!
QJsonArray sd3dRegionWrite( const Sd3drRegion &region );



//!
//! \brief sd3dRegionRead Read region of 3d points from JSON array object
//! \param array          JSON array from which will be readed region
//! \return               Region of 3d points
//!
Sd3drRegion sd3dRegionRead( const QJsonArray &array );



#endif // SD3DRREGION_H
