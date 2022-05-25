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
#ifndef SD3DREGION_H
#define SD3DREGION_H

#include "SvJsonIO.h"

#include <QVector3D>
#include <QList>
#include <QJsonArray>
#include <QMatrix4x4>

using Sd3dRegion = QList<QVector3D>; //! Region of 3d points


void json3dRegion( SvJsonWriter &js, const QString &key, const Sd3dRegion &region );
void json3dRegion( const SvJsonReader &js, const QString &key, Sd3dRegion &region );

//!
//! \brief sd3dRegionWrite Write region of 3d points into JSON array object
//! \param region          Region
//! \return                JSON array object
//!
QJsonArray sd3dRegionWrite( const Sd3dRegion &region );



//!
//! \brief sd3dRegionRead Read region of 3d points from JSON array object
//! \param array          JSON array from which will be readed region
//! \return               Region of 3d points
//!
Sd3dRegion sd3dRegionRead( const QJsonArray &array );



//!
//! \brief regionCircle This function builds circle region on base radius with center at 0
//! \param radius       Radius of builded circle
//! \param stepDegree   Step with which need to create multicorner circle region
//! \return             Circle region on XY plane
//!
Sd3dRegion sd3dRegionCircle(float radius , float stepDegree = 10.0, QVector3D offset = QVector3D(0.0,0.0,0.0) );



//!
//! \brief sd3dRegionCircleSideCount Builds circle region interpolated polygon on base radius with center at 0 and sideCount sides
//! \param radius                    Radius of builded circle
//! \param sideCount                 Side count of polygon
//! \param center                    Center of circle
//! \return                          Circle region on XY plane
//!
Sd3dRegion sd3dRegionCircleSideCount( float radius, int sideCount, QVector3D center );


//!
//! \brief sd3dRegionRectangleSideCount Builds rectangle region with center at 0 and sideCount sides reorganized to rectangle
//! \param w                            Width of rectangle (X)
//! \param h                            Height of rectangle (Y)
//! \param sideCount                    Side count of polygon
//! \param center                       Center of rectangle
//! \return                             Rectangle region on XY plane
//!
Sd3dRegion sd3dRegionRectangleSideCount(float w, float h, int sideCount, QVector3D center);


//!
//! \brief sd3dRegionRectangle Builds rectangle region with center at 0 and four edges
//! \param width               Width of rectangle (X)
//! \param height              Height of rectangle (Y)
//! \param offset              Offset of region
//! \return                    Rectangle region on XY plane
//!
Sd3dRegion sd3dRegionRectangle( float width, float height, QVector3D offset = QVector3D(0.0,0.0,0.0)  );


//!
//! \brief sd3dVertexCenterOfRegion Calculate center of region
//! \param source                   Region for center calculation
//! \return                         Center of region
//!
QVector3D  sd3dVertexCenterOfRegion( const Sd3dRegion &source );


//!
//! \brief sd3dRegionMap Map each point of source region to destignation region through matrix
//! \param source        Source region
//! \param matrix        Matrix of conversion
//! \return              Result region
//!
Sd3dRegion sd3dRegionMap( const Sd3dRegion &source, const QMatrix4x4 &matrix );


//!
//! \brief sd3dRegionMapInPlace Map each point of region in place through matrix
//! \param region               Region
//! \param matrix               Matrix of conversion
//!
void       sd3dRegionMapInPlace( Sd3dRegion &region, const QMatrix4x4 &matrix );

//!
//! \brief sd3dRegionTranslate Translate each point of source region on amount of offset and place it into destignation region
//! \param source              Source region
//! \param offset              Offset vector of translation
//! \return                    Result region
//!
Sd3dRegion sd3dRegionTranslate( const Sd3dRegion &source, QVector3D offset );

//!
//! \brief sd3dRegionShift The function moves the region in the direction perpendicular
//!                        to the plane of the region by the specified shift amount
//! \param source          Source region to move
//! \param shift           Shift amount
//! \return                Shifted region
//!
Sd3dRegion sd3dRegionShift( const Sd3dRegion &source, float shift );

//!
//! \brief sd3dRegionBegin Starts creation of continuously region
//! \param x               Start coordinates of first point of region
//! \param y
//! \return                Region with single point
//!
Sd3dRegion sd3dRegionBegin( float x, float y );

//!
//! \brief sd3dRegionAddX Appends to region vertex shifted regarding last point at x by axis X
//! \param region         Source of region
//! \param x              Regarding coordinate x
//! \return               Region with appended vertex
//!
Sd3dRegion sd3dRegionAddX( const Sd3dRegion &region, float x );


//!
//! \brief sd3dRegionAddY Appends to region vertex shifted regarding last point at y by axis Y
//! \param region         Source of region
//! \param x              Regarding coordinate y
//! \return               Region with appended vertex
//!
Sd3dRegion sd3dRegionAddY( const Sd3dRegion &region, float y );

//!
//! \brief sd3dRegionAddArc Appends arc to end of region from last point of region to stop point
//! \param region           Source of region
//! \param stop             Stop point of arc
//! \param radius           Radius of arc
//! \param direction        Direction of rotation. Positive value for CW and negative for CCW
//! \return                 Region with appended arc
//!
Sd3dRegion sd3dRegionAddArc( const Sd3dRegion &region, QVector3D stop, float radius, float direction );

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
Sd3dRegion sd3dRegionAddArcOffset( const Sd3dRegion &region, float stopOffsetX, float stopOffsetY, float radius, float direction );

#endif // SD3DREGION_H
