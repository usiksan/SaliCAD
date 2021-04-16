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
//! \param center                    Center of circle
//! \return                          Circle region on XY plane
//!
Sd3dRegion sd3dRegionCircleSideCount(float radius, int sideCount, QVector3D center)
  {
  float stepDegree = 360.0 / sideCount;
  return sd3dRegionCircle( radius, stepDegree, center );
  }




//!
//! \brief sd3dRegionRectangleSideCount Builds rectangle region with center at 0 and sideCount sides reorganized to rectangle
//! \param w                            Width of rectangle (X)
//! \param h                            Height of rectangle (Y)
//! \param sideCount                    Side count of polygon
//! \param center                       Center of rectangle
//! \return                             Rectangle region on XY plane
//!
Sd3dRegion sd3dRegionRectangleSideCount(float w, float h, int sideCount, QVector3D center )
  {
  float edgeOnSide = sideCount / 4;
  float stepw = w / edgeOnSide;
  float steph = h / edgeOnSide;
  QVector3D v( 0, h/2, 0);
  v += center;
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




//!
//! \brief sd3dRegionMap Map each point of source region to destignation region through matrix
//! \param source        Source region
//! \param matrix        Matrix of conversion
//! \return              Result region
//!
Sd3dRegion sd3dRegionMap(const Sd3dRegion &source, const QMatrix4x4 &matrix)
  {
  Sd3dRegion dest;
  dest.reserve( source.size() );
  for( auto const &v : source )
    dest.append( matrix.map(v) );
  return dest;
  }



//!
//! \brief sd3dRegionMapInPlace Map each point of region in place through matrix
//! \param region               Region
//! \param matrix               Matrix of conversion
//!
void sd3dRegionMapInPlace(Sd3dRegion &region, const QMatrix4x4 &matrix)
  {
  for( auto &v : region )
    v = matrix.map(v);
  }




//!
//! \brief sd3dRegionRectangle Builds rectangle region with center at 0 and four edges
//! \param width               Width of rectangle (X)
//! \param height              Height of rectangle (Y)
//! \param offset              Offset of region
//! \return                    Rectangle region on XY plane
//!
Sd3dRegion sd3dRegionRectangle( float width, float height, QVector3D offset )
  {
  width /= 2.0;
  height /= 2.0;
  Sd3dRegion region;
  region.append( QVector3D(-width + offset.x(), -height + offset.y(), offset.z() ) );
  region.append( QVector3D(-width + offset.x(), height + offset.y(), offset.z() ) );
  region.append( QVector3D(width + offset.x(), height + offset.y(), offset.z() ) );
  region.append( QVector3D(width + offset.x(), -height + offset.y(), offset.z() ) );
  return region;
  }





//!
//! \brief sd3dRegionTranslate Translate each point of source region on amount of offset and place it into destignation region
//! \param source              Source region
//! \param offset              Offset vector of translation
//! \return                    Result region
//!
Sd3dRegion sd3dRegionTranslate(const Sd3dRegion &source, QVector3D offset)
  {
  Sd3dRegion region;
  for( const auto v : source )
    region.append( v + offset );
  return region;
  }




//!
//! \brief sd3dRegionShift The function moves the region in the direction perpendicular
//!                        to the plane of the region by the specified shift amount
//! \param source          Source region to move
//! \param shift           Shift amount
//! \return                Shifted region
//!
Sd3dRegion sd3dRegionShift(const Sd3dRegion &source, float shift)
  {
  if( source.count() < 3 ) return source;
  //Normal vector
  QVector3D normal = QVector3D::normal( source.at(0), source.at(1), source.at(2) );
  normal *= -shift;
  return sd3dRegionTranslate( source, normal );
  }





//!
//! \brief sd3dRegionBegin Starts creation of continuously region
//! \param x               Start coordinates of first point of region
//! \param y
//! \return                Region with single point
//!
Sd3dRegion sd3dRegionBegin(float x, float y)
  {
  Sd3dRegion region;
  region.append( QVector3D( x, y, 0 ) );
  return region;
  }



//!
//! \brief sd3dRegionAddX Append to region vertex shifted regarding last point at x by axis X
//! \param region         Source of region
//! \param x              Regarding coordinate x
//! \return               Region with appended vertex
//!
Sd3dRegion sd3dRegionAddX(const Sd3dRegion &region, float x)
  {
  Sd3dRegion dest(region);
  QVector3D v( region.last() );
  dest.append( v + QVector3D(x,0,0) );
  return dest;
  }




//!
//! \brief sd3dRegionAddY Append to region vertex shifted regarding last point at y by axis Y
//! \param region         Source of region
//! \param x              Regarding coordinate y
//! \return               Region with appended vertex
//!
Sd3dRegion sd3dRegionAddY(const Sd3dRegion &region, float y)
  {
  Sd3dRegion dest(region);
  QVector3D v( region.last() );
  dest.append( v + QVector3D(0, y, 0) );
  return dest;
  }



//!
//! \brief sd3dRegionAddArc Appends arc to end of region from last point of region to stop point
//! \param region           Source of region
//! \param stop             Stop point of arc
//! \param radius           Radius of arc
//! \param direction        Direction of rotation. Positive value for CW and negative for CCW
//! \return                 Region with appended arc
//!
Sd3dRegion sd3dRegionAddArc(const Sd3dRegion &region, QVector3D stop, float radius, float direction)
  {
  Sd3dRegion dest(region);

  QVector3D start( region.last() );
  float dx = stop.x() - start.x();
  float dy = stop.y() - start.y();

  if( radius == 0 || direction == 0 || (dx == 0 && dy == 0)) {
    //Invalid input params. Arc building impossible. We return simple end point
    dest.append(stop);
    return dest;
    }

  //Calculate center of arc
  float d = sqrt( dx * dx + dy * dy );
  float h = sqrt( radius * radius - (d/2.0) * (d/2.0) );

  QVector3D center;
  if( radius > 0 ) {
    center.setX( start.x() + dx / 2.0 + h * dy / d );
    center.setY( start.y() + dy / 2.0 - h * dx / d );
    }
  else {
    center.setX( start.x() + dx / 2.0 - h * dy / d );
    center.setY( start.y() + dy / 2.0 + h * dx / d );
    }

  float

  }


//!
//! \brief sd3dRegionAddArcOffset Appends arc to end of region from last point of region to stop point
//!                               where stop point defined as last point shifted on offset
//! \param region                 Source of region
//! \param stopOffsetX            Stop point offset X of last point of source region
//! \param stopOffsetY            Stop point offset Y of last point of source region
//! \param radius                 Radius of arc
//! \param direction              Direction of rotation. Positive value for CW and negative for CCW
//! \return                       Region with appended arc
//!
Sd3dRegion sd3dRegionAddArcOffset(const Sd3dRegion &region, float stopOffsetX, float stopOffsetY, float radius, float direction)
  {
  QVector3D v( region.last() );
  return sd3dRegionAddArc( region, v + QVector3D( stopOffsetX, stopOffsetY, 0), radius, direction );
  }
