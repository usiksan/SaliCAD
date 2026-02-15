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


class Sd3drModel
  {
    Sd3drRegion       mVertexList;   //!< 3d vertex list
    Sd3drInstanceList mInstanceList; //!< 3d instance list, each instance contains one or more copy of some body list
  public:

    Sd3ColorList  colorList() const;

    void          colorListSet( const Sd3ColorList &lst );

    //!
    //! \brief clear Clear model
    //!
    void          clear();

    //==============================================================================================
    //  Vertex section - functions for model vertex manipulations
    //

    //!
    //! \brief vertex Retrive vertex by its index
    //! \param index  Index of vertex
    //! \return       Vertex
    //!
    QVector3D     vertex( int index ) const { return mVertexList.at(index); }

    //!
    //! \brief point Projection of vertex to XY face
    //! \param index Index of vertex
    //! \return      2d point
    //!
    QPointF       point( int index ) const { return mVertexList.at(index).toPointF(); }

    Sd3drRegion   vertexList( const QList<int> &indexList ) const;

    int           vertexAppend( QVector3D v ) { mVertexList.append(v); return mVertexList.count() - 1; }

    int           vertexCount() const { return mVertexList.count(); }



    //==============================================================================================
    //  Matrix section - functions for matrix calculations
    //

    QMatrix4x4    matrixShift( const Sd3drFace &face, float shift );



    //==============================================================================================
    //  Face section - functions for faces generation and conversion
    //

    Sd3drFace     faceFromRegion( Sd3drRegion r );

    //!
    //! \brief faceFlat         Builds flat face from list of pairs floats
    //! \param firstVertexIndex First vertex index from which builds face
    //! \param pairList         Pair list as offset from current point begining from firstVertexIndex
    //! \param orientation      0 - xy, 1 - yz, 2 - xz
    //! \return
    //!
    Sd3drFace     faceFlat(int firstVertexIndex, const QList<float> &pairList, int orientation );

    //!
    //! \brief faceFlatMatrix Builds flat face from list of pairs float. First pair is source of path and next pairs are offsets from previous pair
    //! \param pairList       List of pairs float. First pair is source of path and next pairs are offsets from previous pair
    //! \param map            Transfer map
    //! \return               Face
    //!
    Sd3drFace     faceFlatMatrix( const QList<float> &pairList, const QMatrix4x4 &map );

    //!
    //! \brief faceCircle This function builds circle region on base radius with center at 0
    //! \param radius     Radius of builded circle
    //! \param stepDegree Step with which need to create multicorner circle region
    //! \param map        Finish circle transformation map
    //! \return           Circle
    //!
    Sd3drFace     faceCircle( float radius, float stepDegree, const QMatrix4x4 &map );

    //!
    //! \brief faceCircleSide Builds circle region interpolated polygon on base radius with center at 0 and sideCount sides
    //! \param radius         Radius of builded circle
    //! \param sideCount      Side count of polygon
    //! \param map            Finish circle transformation map
    //! \return               Circle
    //!
    Sd3drFace     faceCircleSide( float radius, int sideCount, const QMatrix4x4 &map );

    Sd3drFace     faceEllipse( float radiusx, float radiusy, float stepDegree, const QMatrix4x4 &map );

    Sd3drFace     faceEllipseSide( float radiusx, float radiusy, int sideCount, const QMatrix4x4 &map );

    //!
    //! \brief faceRectangle Builds rectangle region with center at 0 and four edges
    //! \param lenght        Lenght of rectangle (X)
    //! \param width         Width of rectangle (Y)
    //! \param map           Finish rectangle transformation map
    //! \return              Rectangle
    //!
    Sd3drFace     faceRectangle( float lenght, float width, const QMatrix4x4 &map );

    Sd3drFace     faceRectangleRound( float lenght, float width, float radius, float stepDegree, const QMatrix4x4 &map );

    //!
    //! \brief faceFromUmPolygon Builds face from region defined in um
    //! \param poly              Source region
    //! \param map               Map to convert result
    //! \return                  Face
    //!
    Sd3drFace     faceFromUmPolygon( const QPolygonF &poly, const QMatrix4x4 &map );

    //!
    //! \brief faceRectangleSide Builds rectangle region with center at 0 and sideCount sides reorganized to rectangle
    //! \param width             Lenght of rectangle (X)
    //! \param lenght            Width of rectangle (Y)
    //! \param sideCount         Side count of polygon
    //! \param map               Finish rectangle transformation map
    //! \return                  Rectangle
    //!
    Sd3drFace     faceRectangleSide( float lenght, float width, int sideCount, const QMatrix4x4 &map );

    Sd3drFace     faceDuplicate( const Sd3drFace &face, const QMatrix4x4 &map );

    Sd3drFace     faceDuplicateShift( const Sd3drFace &face, float shift ) { return faceDuplicate( face, matrixShift( face, shift ) ); }

    Sd3drFace     faceDuplicateScale( const Sd3drFace &face, float scalex, float scaley, float shift );

    Sd3drFace     faceDuplicateGrow( const Sd3drFace &face, float deltax, float deltay, const QMatrix4x4 &map );

    Sd3drFace     faceTrapezoidZ( const Sd3drFace &face, float height, float lessX, float lessY, float topShiftX, float topShiftY );

    Sd3drFace     faceCurveXZ( const Sd3drFace &face, float radius, float angleSrc, float angleDst );

    //!
    //! \brief faceEqudistante Calculate equidistant face of give face with distance. At last make transformation with map
    //! \param face            Source face
    //! \param distance        Distance of new face
    //! \param map             Finish translation map
    //! \return                Equidistant face
    //!
    Sd3drFace     faceEqudistanteXY( const Sd3drFace &face, float distance, const QMatrix4x4 &map );

    //!
    //! \brief faceBevelXY Calculate finish round face of given face with radius and with shift with given direction
    //! \param face        Source face
    //! \param radius      Distance of new face
    //! \return            Shifted round face
    //!
    Sd3drFace     faceBevelXY(const Sd3drFace &face, float radius);

    static Sd3drFace     facePart( const Sd3drFace &face, const QList<float> &indexes );



    //==============================================================================================
    //  Face list section - functions for face list generation and conversion
    //


    Sd3drFaceList faceListDuplicate(const Sd3drFaceList &faceList, const QMatrix4x4 &map );

    Sd3drFaceList faceListSimplifyXY(const Sd3drFace &face );

    Sd3drFaceList faceListHolesXY(const Sd3drFace &srcFace, const Sd3drFaceList &holeList );

    //!
    //! \brief faceListExtrude Extrudes model from source face in direction specified by map
    //! \param face            Source face of model
    //! \param map             Direction of extruding
    //! \return                Solid model: floor, roof and walls
    //!
    Sd3drFaceList faceListExtrude( const Sd3drFace &face, const QMatrix4x4 &map );

    //!
    //! \brief faceListExtrudeShift Extrudes model from region in the direction of the normal vector with
    //!                             specified shift amount color faces.
    //!                             First face is bottom (begin of vector), last face is top (end of vector),
    //!                             Middle faces are walls.
    //! \param face                 Region of bottom of model
    //! \param shift                Shift amount of extrude
    //! \return                     Solid model extruded from region in the direction of the normal vector
    //!
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

    //!
    //! \brief faceListBox  Builds box model from its size
    //! \param lenght       Lenght of box (x)
    //! \param width        Width of box (y)
    //! \param height       Height of box (z)
    //! \param map          Mapping matrix for box
    //! \return             Box model
    //!
    Sd3drFaceList faceListBox(float lenght, float width, float height , const QMatrix4x4 &map);

    //!
    //! \brief faceListCylinder  Builds cylinder model from its size
    //! \param radius            Radius of circle of footing of cylinder
    //! \param height            Height of cylinder
    //! \return                  Cylinder model
    //!
    Sd3drFaceList faceListCylinder( float radius, float height, const QMatrix4x4 &map );

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



    Sd3drFaceList faceListCurveXZ(const Sd3drFace &face, float radius, float angleSrc, float angleDst, int sideCount , bool close);


    Sd3drFaceList faceListPinCurveOne(const Sd3drFace &face, float firstLen, float middleLen, float radius, float angle, int sideCount );

    Sd3drFaceList faceListPinCurveTwo(const Sd3drFace &face, float firstLen, float middleLen, float lastLen, float radius, float angleFirst, float angleSecond, int sideCount );


    Sd3drFaceList faceListPinTqfp(float width, float thickness, float fullLenght, float plateLenght, float height );

    Sd3drFaceList faceListWallRound(const Sd3drFace &face1, const Sd3drFace &face2, float scaleX, float scaleY, float radius, float stepDegree );

    Sd3drFaceList faceListWallBevelXY( const Sd3drFace &face1, const Sd3drFace &face2, float radius, float stepDegree );

    Sd3drFaceList faceListWallDoubleBevelXY(const Sd3drFace &face1, const Sd3drFace &face2, float radius1, float radius2, float stepDegree1, float stepDegree2, float height );

    Sd3drFaceList faceListRotation(const QList<float> &pairs, float angleStart, float angleStop , int sideCount, QMatrix4x4 transfer);

    //!
    //! \brief faceListWalls Builds walls on base layers, count of element each of them is equals each other
    //! \param layers        List of faces throught which will be builds walls
    //! \param close         If true then append wall with n-1 and 0 index vertex on each layer
    //! \return              List of walls
    //!
    static Sd3drFaceList faceListWalls( const Sd3drFaceList &layers, bool close );

    //!
    //! \brief faceListWalls  Builds walls on base bottom and top regions. Walls builded with color
    //! \param face1          First bound of walls - Bottom
    //! \param face2          Second bound of walls - Top
    //! \param close          If true then append wall with n-1 and 0 index vertex
    //! \return               List of walls
    //!
    static Sd3drFaceList faceListWall( const Sd3drFace &face1, const Sd3drFace &face2, bool close );

    static Sd3drFaceList faceListWallList( const Sd3drFaceList &faceList1, const Sd3drFaceList &faceList2, bool close );

    static Sd3drFaceList faceListWallIndexed( const Sd3drFace &face1, const Sd3drFace &face2, const QList<float> &indexes );

    static Sd3drFaceList faceListIndexed( const Sd3drFaceList &faceList, const QList<float> &indexes );

    static QList<float>  afloatArc(float radius, float angleStart, float angleStop, int sideCount);


    //==============================================================================================
    //  Misc section - some service functions
    //

    //!
    //! \brief instanceAppend Appends instance to instance list
    //! \param inst           Instance to append
    //!
    void       instanceAppend( const Sd3drInstance &inst ) { mInstanceList.append(inst); }


    //!
    //! \brief json Overloaded function to write object content into json writer
    //!             Overrided function
    //! \param js   Json writer
    //!
    void       json( SvJsonWriter3d &js ) const;

    //!
    //! \brief json Overloaded function to read object content from json reader
    //!             Overrided function
    //! \param js   Json reader
    //!
    void       json(const SvJsonReader3d &js );

    //!
    //! \brief draw3d Draws model in 3d space
    //! \param f      OpenGL functions
    //! \param mat    Conversion matrix for model
    //!
    void       draw3d(QOpenGLFunctions_2_0 *f, QMatrix4x4 mat = QMatrix4x4() ) const;

    //!
    //! \brief volumeAdd Append volume of model to result volume
    //! \param volume    Source and result volume
    //!
    void       volumeAdd( QMatrix2x3 &volume ) const;

  private:
    void       faceSizeXY( const Sd3drFace &face, float &sizex, float &sizey ) const;

  };

#endif // SD3DMODEL_H
