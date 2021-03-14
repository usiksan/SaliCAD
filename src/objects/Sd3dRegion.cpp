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
#include "Sd3dRegion.h"
#include "Sd3dPoint.h"

#include <math.h>


//!
//! \brief sd3dRegionWrite Write region of 3d points into JSON array object
//! \param region          Region
//! \return                JSON array object
//!
//! We write each point as three separate values of coordinates.
//! Coordinates convert to int mcm to reduce output JSON text volume.
QJsonArray sd3dRegionWrite(const Sd3dRegion &region)
  {
  QJsonArray array;
  for( auto const &pt : region ) {
    array.append( floatMmToIntMcm(pt.x()) );
    array.append( floatMmToIntMcm(pt.y()) );
    array.append( floatMmToIntMcm(pt.z()) );
    }
  return array;
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
Sd3dRegion sd3dRegionRead(const QJsonArray &array)
  {
  Sd3dRegion region;
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





//!
//! \brief regionCircle This function builds circle region on base radius with center at 0
//! \param radius       Radius of builded circle
//! \param stepDegree   Step with which need to create multicorner circle region
//! \return             Circle region on XY plane
//!
Sd3dRegion sd3dRegionCircle(float radius, float stepDegree, QVector3D offset)
  {
  Sd3dRegion region;
  //Build circle with step 10 degree
  for( float angleDegree = 0; angleDegree < 360.0; angleDegree += stepDegree ) {
    //Convert degree to radians
    float angle = angleDegree * M_PI / 180.0;
    //Build next corner
    QVector3D v( sin(angle) * radius, cos(angle) * radius, 0 );
    //Append corner to region
    region.append( v + offset );
    }
  return region;
  }




//!
//! \brief sd3dRegionCircleSideCount Builds circle region interpolated polygon on base radius with center at 0 and sideCount sides
//! \param radius                    Radius of builded circle
//! \param sideCount                 Side count of polygon
//! \return                          Circle region on XY plane
//!
Sd3dRegion sd3dRegionCircleSideCount(float radius, int sideCount)
  {
  float stepDegree = 360.0 / sideCount;
  return sd3dRegionCircle( radius, stepDegree );
  }




//!
//! \brief sd3dRegionRectangleSideCount Builds rectangle region with center at 0 and sideCount sides reorganized to rectangle
//! \param w                            Width of rectangle (X)
//! \param h                            Height of rectangle (Y)
//! \param sideCount                    Side count of polygon
//! \return                             Rectangle region on XY plane
//!
Sd3dRegion sd3dRegionRectangleSideCount(float w, float h, int sideCount)
  {
  float edgeOnSide = sideCount / 4;
  float stepw = w / edgeOnSide;
  float steph = h / edgeOnSide;
  QVector3D v( 0, h/2, 0);
  Sd3dRegion region;
  region.append( v );
  for( int i = 0; i < edgeOnSide / 2; i++ ) {
    v += QVector3D( stepw, 0, 0 );
    region.append( v );
    }
  for( int i = 0; i < edgeOnSide; i++ ) {
    v += QVector3D( 0, -steph, 0 );
    region.append( v );
    }
  for( int i = 0; i < edgeOnSide; i++ ) {
    v += QVector3D( -stepw, 0, 0 );
    region.append( v );
    }
  for( int i = 0; i < edgeOnSide; i++ ) {
    v += QVector3D( 0, steph, 0 );
    region.append( v );
    }
  for( int i = 0; i < edgeOnSide / 2; i++ ) {
    v += QVector3D( stepw, 0, 0 );
    region.append( v );
    }
  return region;
  }





//!
//! \brief sd3dRegionEquidistant Creates region equidistant to source region on distance
//! \param source                Source region
//! \param distance              Distance of equidistant greater zero - outer, less zero - inner
//! \return                      Region equidistant to source region on distance
//!
Sd3dRegion sd3dRegionEquidistant(const Sd3dRegion &source, float distance)
  {

  }




//!
//! \brief sd3dVertexCenterOfRegion Calculate center of region
//! \param source                   Region for center calculation
//! \return                         Center of region
//!
QVector3D sd3dVertexCenterOfRegion(const Sd3dRegion &source)
  {
  QVector3D center;
  for( auto const &v : source )
    center += v;
  float count = source.count();
  return center / count;
  }
