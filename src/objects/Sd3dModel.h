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

#include "Sd3drInstance.h"

class Sd3dModel
  {
    Sd3dRegion        mVertexList;   //!< 3d vertex list
    Sd3drInstanceList mInstanceList; //!< 3d instance list, each instance contains one or more copy of some body list
  public:
    //!
    //! \brief clear Clear model
    //!
    void          clear();

    QVector3D     vertex( int index ) const { return mVertexList.at(index); }

    Sd3dRegion    vertexList( const QList<int> &indexList ) const;

    int           vertexAppend( QVector3D v ) { mVertexList.append(v); return mVertexList.count() - 1; }



    QMatrix4x4    matrixShift( const Sd3drFace &face, float shift );



    Sd3drFace     faceFromRegion( Sd3dRegion r );

    Sd3drFace     faceFlat( const QList<float> &pairList, int orientation );

    Sd3drFace     faceCircle( float radius, float stepDegree, const QMatrix4x4 &map );

    Sd3drFace     faceCircleSide( float radius, int sideCount, const QMatrix4x4 &map );

    Sd3drFace     faceRectangle( float lenght, float width, const QMatrix4x4 &map );

    Sd3drFace     faceRectangleRound( float width, float lenght, float radius, float stepDegree, const QMatrix4x4 &map );

    Sd3drFace     faceRectangleSide( float width, float lenght, int sideCount, const QMatrix4x4 &map );

    Sd3drFace     faceDuplicate( const Sd3drFace &face, const QMatrix4x4 &map );

    Sd3drFace     faceDuplicateShift( const Sd3drFace &face, float shift ) { return faceDuplicate( face, matrixShift( face, shift ) ); }

    Sd3drFace     faceTrapezoidZ( const Sd3drFace &face, float height, float lessX, float lessY, float topShiftX, float topShiftY );

    Sd3drFaceList faceListExtrude( const Sd3drFace &face, const QMatrix4x4 &map );

    Sd3drFaceList faceListExtrudeShift( const Sd3drFace &face, float shift );

    //!
    //! \brief faceListBodyBeveled  Builds beveled body of part
    //! \param bodyLenght           Full body lenght (X)
    //! \param bodyWidth            Full body width (Y)
    //! \param bodyHeight           Full body height (H)
    //! \param bevelFront           Bevel size in front of body
    //! \param bevelSide            Bevel size in other sides of body
    //! \param verticalHeight       Vertical walls height of body for pin placing
    //! \return                     Model of beveled body of part
    //!
    Sd3drFaceList faceListBodyBeveled(float bodyLenght, float bodyWidth, float bodyHeight, float bevelFront, float bevelSide, float verticalHeight );

    Sd3drFaceList faceListBox( float bodyLenght, float bodyWidth, float bodyHeight );

    Sd3drFaceList faceListCylinder( float radius, float height );

    //!
    //! \brief sd3dModelHexagon Builds hexagonal box body of part
    //! \param lenght           Lenght of box
    //! \param topLenght        Top lenght excluding bevels
    //! \param height           Height of box
    //! \param width            Width of box
    //! \param color            Color of faces of model
    //! \return                 Model of hexagonal box body of part
    //!
    Sd3drFaceList faceListHexagon(float lenght, float topLenght, float height, float width);


    static Sd3drFaceList faceListWall( const Sd3drFace &face1, const Sd3drFace &face2, bool close );


    void          instanceAppend( const Sd3drInstance &inst ) { mInstanceList.append(inst); }


    //!
    //! \brief json Overloaded function to write object content into json writer
    //!             Overrided function
    //! \param js   Json writer
    //!
    void       json( SdJsonWriter &js ) const;

    //!
    //! \brief json Overloaded function to read object content from json reader
    //!             Overrided function
    //! \param js   Json reader
    //!
    void       json( const SdJsonReader &js );

    void       draw3d(QOpenGLFunctions_2_0 *f) const;

  };




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
//! \brief sd3dModelTranslateColor Translates mode to other model with offset and changes color of all faces to color
//! \param model                   Source model to translate
//! \param offset                  Offset of translation
//! \param color                   New color of translated faces
//! \return                        Translated and colored model
//!
Sd3dModel  sd3dModelTranslateColor( const Sd3dModel &model, QVector3D offset, QColor color );


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
Sd3dModel  sd3dModelCurve(const Sd3dRegion &region, QVector3D rotationCenter, QVector3D rotationAxis, float angle , float stepAngle, QColor color, bool bottomFace);


//!
//! \brief sd3dModelFlatHole Builds model of flat surface with hole. Outer of surface and hole must contains equals count of edges
//! \param outer             Outer of surface
//! \param hole              Hole
//! \param color             Color of model
//! \return                  Model with hole
//!
Sd3dModel  sd3dModelFlatHole( const Sd3dRegion &outer, const Sd3dRegion &hole, QColor color );


//!
//! \brief sd3dModelBox Builds box model from its size
//! \param lenght       Lenght of box (x)
//! \param width        Width of box (y)
//! \param height       Height of box (z)
//! \param color        Color faces of box
//! \return             Box model
//!
Sd3dModel  sd3dModelBox( float lenght, float width, float height, QColor color );


//!
//! \brief sd3dModelVolume Append model volume to volume matrix
//! \param model           Model which volume appended
//! \param volume          Source and destignation volume
//!
void       sd3dModelVolume(const Sd3dModel &model, QMatrix2x3 &volume );


//!
//! \brief sd3dModelPinTqfp Builds model of pin of tqfp like part
//! \param width            Width of pin
//! \param thickness        Thickness of pin
//! \param fullLenght       Full lenght of pin from body to end of pin
//! \param plateLenght      Plate lenght of pin is a part of pin which apply to plate
//! \param height           Height of pin
//! \param color            Color of faces of model
//! \return                 Model of pin of tqfp like part
//!
Sd3dModel  sd3dModelPinTqfp(float width, float thickness, float fullLenght, float plateLenght, float height , QColor color);


//!
//! \brief sd3dModelHexagon Builds hexagonal box body of part
//! \param lenght           Lenght of box
//! \param topLenght        Top lenght excluding bevels
//! \param height           Height of box
//! \param width            Width of box
//! \param color            Color of faces of model
//! \return                 Model of hexagonal box body of part
//!
Sd3dModel  sd3dModelHexagon( float lenght, float topLenght, float height, float width, QColor color );


//!
//! \brief sd3dModelBodyBeveled Builds beveled body of part
//! \param bodyLenght           Full body lenght (X)
//! \param bodyWidth            Full body width (Y)
//! \param bodyHeight           Full body height (H)
//! \param bevelFront           Bevel size in front of body
//! \param bevelSide            Bevel size in other sides of body
//! \param verticalHeight       Vertical walls height of body for pin placing
//! \param color                Body color
//! \return                     Model of beveled body of part
//!
Sd3dModel  sd3dModelBodyBeveled( float bodyLenght, float bodyWidth, float bodyHeight, float bevelFront, float bevelSide, float verticalHeight, QColor color );

#endif // SD3DMODEL_H
