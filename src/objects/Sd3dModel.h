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

    Sd3ColorList  colorList() const;

    void          colorListSet( const Sd3ColorList &lst );

    //!
    //! \brief clear Clear model
    //!
    void          clear();

    QVector3D     vertex( int index ) const { return mVertexList.at(index); }

    QPointF       point( int index ) const { return mVertexList.at(index).toPointF(); }

    Sd3dRegion    vertexList( const QList<int> &indexList ) const;

    int           vertexAppend( QVector3D v ) { mVertexList.append(v); return mVertexList.count() - 1; }

    int           vertexCount() const { return mVertexList.count(); }



    QMatrix4x4    matrixShift( const Sd3drFace &face, float shift );



    Sd3drFace     faceFromRegion( Sd3dRegion r );

    //!
    //! \brief faceFlat         Builds flat face from list of pairs floats
    //! \param firstVertexIndex First vertex index from which builds face
    //! \param pairList         Pair list as offset from current point begining from firstVertexIndex
    //! \param orientation      0 - xy, 1 - yz, 2 - xz
    //! \return
    //!
    Sd3drFace     faceFlat(int firstVertexIndex, const QList<float> &pairList, int orientation );

    Sd3drFace     faceCircle( float radius, float stepDegree, const QMatrix4x4 &map );

    Sd3drFace     faceCircleSide( float radius, int sideCount, const QMatrix4x4 &map );

    Sd3drFace     faceRectangle( float lenght, float width, const QMatrix4x4 &map );

    Sd3drFace     faceRectangleRound( float lenght, float width, float radius, float stepDegree, const QMatrix4x4 &map );

    Sd3drFace     faceRectangleSide( float width, float lenght, int sideCount, const QMatrix4x4 &map );

    Sd3drFace     faceDuplicate( const Sd3drFace &face, const QMatrix4x4 &map );

    Sd3drFace     faceDuplicateShift( const Sd3drFace &face, float shift ) { return faceDuplicate( face, matrixShift( face, shift ) ); }

    Sd3drFace     faceDuplicateScale( const Sd3drFace &face, float scalex, float scaley, float shift );

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

    static Sd3drFace     facePart( const Sd3drFace &face, const QList<float> &indexes );


    Sd3drFaceList faceListSimplify(const Sd3drFace &face );

    Sd3drFaceList faceListHoles(const Sd3drFace &srcFace, Sd3drFaceList &holeList );

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
    //! \param color        Color faces of box
    //! \return             Box model
    //!
    Sd3drFaceList faceListBox( float lenght, float width, float height );

    //!
    //! \brief faceListCylinder  Builds cylinder model from its size
    //! \param radius            Radius of circle of footing of cylinder
    //! \param height            Height of cylinder
    //! \return                  Cylinder model
    //!
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



    Sd3drFaceList faceListCurveXZ(const Sd3drFace &face, float radius, float angleSrc, float angleDst, int sideCount , bool close);


    Sd3drFaceList faceListPinCurveOne(const Sd3drFace &face, float firstLen, float middleLen, float radius, float angle, int sideCount );

    Sd3drFaceList faceListPinCurveTwo(const Sd3drFace &face, float firstLen, float middleLen, float lastLen, float radius, float angleFirst, float angleSecond, int sideCount );


    Sd3drFaceList faceListPinTqfp(float width, float thickness, float fullLenght, float plateLenght, float height );

    Sd3drFaceList faceListWallRound(const Sd3drFace &face1, const Sd3drFace &face2, float scaleX, float scaleY, float radius, float stepDegree );

    //!
    //! \brief faceListWalls  Builds walls on base bottom and top regions. Walls builded with color
    //! \param face1          First bound of walls - Bottom
    //! \param face2          Second bound of walls - Top
    //! \param close          If true then append wall with n-1 and 0 index vertex
    //! \return               List of walls
    //!
    static Sd3drFaceList faceListWall( const Sd3drFace &face1, const Sd3drFace &face2, bool close );

    static Sd3drFaceList faceListWallIndexed( const Sd3drFace &face1, const Sd3drFace &face2, const QList<float> &indexes );

    static Sd3drFaceList faceListIndexed( const Sd3drFaceList &faceList, const QList<float> &indexes );


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

    void       build( const QString &script );

    //!
    //! \brief volumeAdd Append volume of model to result volume
    //! \param volume    Source and result volume
    //!
    void       volumeAdd( QMatrix2x3 &volume ) const;

  private:
    int        lessLeft( const Sd3drFace &face ) const;

    bool       isLeft( QPointF p1, QPointF p2 ) const;

    int        next( int center, const Sd3drFace &face ) const;

    int        prev( int center, const Sd3drFace &face ) const;

  };


#endif // SD3DMODEL_H
