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

#include <QVector3D>
#include <QList>
#include <QJsonArray>
#include <QMatrix4x4>

using Sd3dRegion = QList<QVector3D>; //! Region of 3d points



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

#endif // SD3DREGION_H
