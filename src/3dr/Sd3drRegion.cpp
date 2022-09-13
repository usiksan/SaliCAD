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
#include "Sd3drRegion.h"

#include <math.h>


//!
//! \brief sd3dRegionWrite Write region of 3d points into JSON array object
//! \param region          Region
//! \return                JSON array object
//!
//! We write each point as three separate values of coordinates.
//! Coordinates convert to int mcm to reduce output JSON text volume.
QJsonArray sd3dRegionWrite(const Sd3drRegion &region)
  {
  QJsonArray array;
  for( auto const &pt : region ) {
    array.append( floatMmToIntMcm(pt.x()) );
    array.append( floatMmToIntMcm(pt.y()) );
    array.append( floatMmToIntMcm(pt.z()) );
    }
  return array;
  }

void json3dRegion(SvJsonWriter &js, const QString &key, const Sd3drRegion &region)
  {
  js.object().insert( key, sd3dRegionWrite(region) );
  }




//!
//! \brief sd3dRegionRead Read region of 3d points from JSON array object
//! \param array          JSON array from which will be readed region
//! \return               Region of 3d points
//!
//! We write each point as three separate values of coordinates.
//! Coordinates convert to int mcm to reduce output JSON text volume.
//! So, when we read we convert each coordinate to float mm and
//! pach each three continuous coordinates into single 3d vertex
Sd3drRegion sd3dRegionRead(const QJsonArray &array)
  {
  Sd3drRegion region;
  region.reserve( array.count() / 3 );
  QVector3D pt;
  int i = 0;
  for( auto it = array.constBegin(); it != array.constEnd(); it++ ) {
    switch(i) {
      case 0 : pt.setX( intMcmToFloatMm(it->toInt()) ); break;
      case 1 : pt.setY( intMcmToFloatMm(it->toInt()) ); break;
      case 2 : pt.setZ( intMcmToFloatMm(it->toInt()) ); break;
      }
    i++;
    if( i > 2 ) {
      region.append( pt );
      i = 0;
      }
    }
  return region;
  }


void json3dRegion( const SvJsonReader &js, const QString &key, Sd3drRegion &region )
  {
  region = sd3dRegionRead( js.object().value(key).toArray() );
  }



