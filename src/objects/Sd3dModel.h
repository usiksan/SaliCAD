/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  3d object which represents face set model (Model which consists of face set)
  Each face is flate surface bounded polyline
  With comparison Sd3dFaceSet this object contains simplified faces (no normal, single face color)
*/
#ifndef SD3DMODEL_H
#define SD3DMODEL_H

#include "Sd3dFace.h"

using Sd3dModel = QList<Sd3dFace>;

//!
//! \brief sd3dModelWrite Writes model into JSON array object
//! \param model          3d model
//! \return               JSON array object with writed 3d model
//!
QJsonArray sd3dModelWrite( const Sd3dModel &model );


//!
//! \brief sd3dModelRead Reads model from JSON array object
//! \param array         JSON array object from which will be readed 3d model
//! \return              3d model
//!
Sd3dModel  sd3dModelRead( const QJsonArray &array );


//!
//! \brief sd3dModelWalls Builds walls on base bottom and top regions. Walls builded with color
//! \param bottom         Bottom region of walls
//! \param top            Top region of walls
//! \param color          Color of faces for the walls
//! \param close          If true then append wall with n-1 and 0 index vertex
//! \return               Model of walls
//!
Sd3dModel  sd3dModelWalls( const Sd3dRegion &bottom, const Sd3dRegion &top, QColor color, bool close );


//!
//! \brief sd3dModelWall Builds walls on base bottom region and vector of grow. Walls builded with color
//! \param bottom        Bottom region of walls
//! \param grow
//! \param color         Color of faces for the walls
//! \param close         If true then append wall with n-1 and 0 index vertex
//! \return              Model of walls
//!
Sd3dModel  sd3dModelWall( const Sd3dRegion &bottom, QVector3D grow, QColor color, bool close );


//!
//! \brief sd3dModelExtrude Extrudes model from region in the direction of the normal vector with
//!                         specified shift amount color faces.
//!                         First face is bottom (begin of vector), last face is top (end of vector),
//!                         Middle faces are walls.
//! \param region           Region of bottom of model
//! \param shift            Shift amount of extrude
//! \param color            Face model color
//! \return                 Model extruded from region in the direction of the normal vector
//!
Sd3dModel  sd3dModelExtrude( const Sd3dRegion &region, float shift, QColor color );


//!
//! \brief sd3dModelCylinder Builds cylinder model from its size
//! \param radius            Radius of circle of footing of cylinder
//! \param height            Height of cylinder
//! \param color             Color of cylinder
//! \return                  Cylinder model
//!
Sd3dModel  sd3dModelCylinder( float radius, float height, QColor color );


//!
//! \brief sd3dModelWallEven Builds wall from each pair of vertexes with grow vector and face color
//! \param regionPair        Region pair contains pair of vertexes (segments), count of vertexes must be even
//! \param grow              Wall grow vector
//! \param color             Face color
//! \return                  Model of walls
//!
Sd3dModel  sd3dModelWallEven( const Sd3dRegion &regionPair, QVector3D grow, QColor color);


//!
//! \brief sd3dModelTranslate Translates mode to other model with offset
//! \param model              Source model to translate
//! \param offset             Offset of translation
//! \return                   Translated model
//!
Sd3dModel  sd3dModelTranslate( const Sd3dModel &model, QVector3D offset );


//!
//! \brief sd3dModelMap Convert source model to destignation with matrix
//! \param model        Source model
//! \param matrix       Conversion matrix
//! \return             Converted model
//!
Sd3dModel  sd3dModelMap( const Sd3dModel &model, const QMatrix4x4 &matrix );


//!
//! \brief sd3dModelMapInPlace Convert model in place using matrix
//! \param model               Model to convert
//! \param matrix              Conversion matrix
//!
void       sd3dModelMapInPlace( Sd3dModel &model, const QMatrix4x4 &matrix );


//!
//! \brief sd3dModelCurve Creates curve solid model from base region and rotation angle
//! \param region         Source region which will be form curved model
//! \param rotationCenter Center of rotation
//! \param rotationAxis   Axis of rotation
//! \param angle          Angle of rotation
//! \param color          Faces color
//! \param bottomFace     If true then appended bottom face from source region
//! \return               Curved solid model
//!
Sd3dModel  sd3dModelCurve( const Sd3dRegion &region, QVector3D rotationCenter, QVector3D rotationAxis, float angle , QColor color, bool bottomFace);


//!
//! \brief sd3dModelFlatHole Builds model of flat surface with hole. Outer of surface and hole must contains equals count of edges
//! \param outer             Outer of surface
//! \param hole              Hole
//! \param color             Color of model
//! \return                  Model with hole
//!
Sd3dModel  sd3dModelFlatHole( const Sd3dRegion &outer, const Sd3dRegion &hole, QColor color );

#endif // SD3DMODEL_H
