#include "Sd3dModel.h"


#include <math.h>





//!
//! \brief sd3dModelWalls Builds walls on base bottom and top regions. Walls builded with color
//! \param bottom         Bottom region of walls
//! \param top            Top region of walls
//! \param color          Color of faces for the walls
//! \param close          If true then append wall with n-1 and 0 index vertex
//! \return               Model of walls
//!
Sd3dModel sd3dModelWalls(const Sd3dRegion &bottom, const Sd3dRegion &top, QColor color, bool close)
  {
  Sd3dModel model;
  Sd3dFace face;
  face.mColor = color;
  for( int i = 0; i < bottom.count() - 1; i++ ) {
    face.mContour.clear();
    face.mContour.append( bottom.at(i) );
    face.mContour.append( bottom.at(i+1) );
    face.mContour.append( top.at(i+1) );
    face.mContour.append( top.at(i) );
    model.append( face );
    }
  if( close ) {
    //Append wall with n-1 and 0 vertex
    face.mContour.clear();
    face.mContour.append( bottom.last() );
    face.mContour.append( bottom.first() );
    face.mContour.append( top.first() );
    face.mContour.append( top.last() );
    model.append( face );
    }
  return model;
  }


//!
//! \brief sd3dModelWall Builds walls on base bottom region and vector of grow. Walls builded with color
//! \param bottom        Bottom region of walls
//! \param grow
//! \param color         Color of faces for the walls
//! \param close         If true then append wall with n-1 and 0 index vertex
//! \return              Model of walls
//!
Sd3dModel sd3dModelWall( const Sd3dRegion &bottom, QVector3D grow, QColor color, bool close)
  {
  return sd3dModelWalls( bottom, sd3dRegionTranslate( bottom, grow ), color, close );
  }


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
Sd3dModel sd3dModelExtrude(const Sd3dRegion &region, float shift, QColor color)
  {
  Sd3dModel md;
  Sd3dFace bot( region, color );

  //Bottom side
  md.append( bot );

  Sd3dFace top( bot.shift( shift ) );

  //Side walls
  md.append( sd3dModelWalls( region, top.mContour, color, true ) );

  //Top side
  md.append( top );

  return md;
  }


//!
//! \brief sd3dModelCylinder Builds cylinder model from its size
//! \param radius            Radius of circle of footing of cylinder
//! \param height            Height of cylinder
//! \param color             Color of cylinder
//! \return                  Cylinder model
//!
Sd3dModel sd3dModelCylinder(float radius, float height, QColor color)
  {
  return sd3dModelExtrude( sd3dRegionCircle( radius ), height, color );
  }





//!
//! \brief sd3dModelWallEven Builds wall from each pair of vertexes with grow vector and face color
//! \param regionPair        Region pair contains pair of vertexes (segments), count of vertexes must be even
//! \param grow              Wall grow vector
//! \param color             Face color
//! \return                  Model of walls
//!
Sd3dModel sd3dModelWallEven( const Sd3dRegion &regionPair, QVector3D grow, QColor color)
  {
  Sd3dRegion otherRegion( sd3dRegionTranslate( regionPair, grow ) );
  Sd3dModel model;
  model.reserve( regionPair.count() / 2 );
  Sd3dFace face;
  face.mColor = color;
  for( int i = 0; regionPair.count() - 1; i += 2 ) {
    face.mContour.clear();
    face.mContour.append( regionPair.at(i) );
    face.mContour.append( regionPair.at(i+1) );
    face.mContour.append( otherRegion.at(i+1) );
    face.mContour.append( otherRegion.at(i) );
    model.append( face );
    }
  return model;
  }




//!
//! \brief sd3dModelTranslate Translates mode to other model with offset
//! \param model              Source model to translate
//! \param offset             Offset of translation
//! \return                   Translated model
//!
Sd3dModel sd3dModelTranslate(const Sd3dModel &model, QVector3D offset)
  {
  Sd3dModel dest;
  dest.reserve( model.count() );
  for( auto const &face : model )
    dest.append( face.translate( offset ) );
  return dest;
  }




//!
//! \brief sd3dModelTranslateColor Translates mode to other model with offset and changes color of all faces to color
//! \param model                   Source model to translate
//! \param offset                  Offset of translation
//! \param color                   New color of translated faces
//! \return                        Translated and colored model
//!
Sd3dModel sd3dModelTranslateColor(const Sd3dModel &model, QVector3D offset, QColor color)
  {
  Sd3dModel dest;
  dest.reserve( model.count() );
  for( auto const &face : model )
    dest.append( face.translateColor( offset, color ) );
  return dest;
  }




//!
//! \brief sd3dModelMap Convert source mode to destignation with matrix
//! \param model        Source model
//! \param matrix       Conversion matrix
//! \return             Converted model
//!
Sd3dModel sd3dModelMap(const Sd3dModel &model, const QMatrix4x4 &matrix)
  {
  Sd3dModel dest;
  dest.reserve( model.count() );
  for( auto const &face : model )
    dest.append( face.map( matrix ) );
  return dest;
  }




//!
//! \brief sd3dModelMapInPlace Convert model in place using matrix
//! \param model               Model to convert
//! \param matrix              Conversion matrix
//!
void sd3dModelMapInPlace(Sd3dModel &model, const QMatrix4x4 &matrix)
  {
  for( auto &face : model )
    face.mapInPlace( matrix );
  }




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
Sd3dModel sd3dModelCurve(const Sd3dRegion &region, QVector3D rotationCenter, QVector3D rotationAxis, float angle, float stepAngle, QColor color, bool bottomFace )
  {
  Sd3dModel model;

  //Bottom face
  if( bottomFace )
    model.append( Sd3dFace( region, color ) );

  //Walls
  Sd3dRegion bot( region );
  if( angle > 0 ) {
    float addon = stepAngle;
    do {
      //Build matrix to rotate to current angle
      QMatrix4x4 matrix;
      matrix.translate( -rotationCenter );
      matrix.rotate( qMin( addon, angle ), rotationAxis );
      matrix.translate( rotationCenter );

      //Build rotated redion
      Sd3dRegion top;
      for( auto const &v : qAsConst(region) )
        top.append( matrix.map( v ) );

      //Append walls
      model.append( sd3dModelWalls( bot, top, color, true ) );

      bot = top;
      addon += stepAngle;
      }
    while( addon <= angle );
    }
  else {
    float addon = -stepAngle;
    do {
      //Build matrix to rotate to current angle
      QMatrix4x4 matrix;
      matrix.translate( -rotationCenter );
      matrix.rotate( qMax( addon, angle ), rotationAxis );
      matrix.translate( rotationCenter );

      //Build rotated redion
      Sd3dRegion top;
      for( auto const &v : region )
        top.append( matrix.map( v ) );

      //Append walls
      model.append( sd3dModelWalls( bot, top, color, true ) );

      bot = top;
      addon -= stepAngle;
      }
    while( addon >= angle );
    }

  //Top face
  model.append( Sd3dFace( bot, color ) );

  return model;
  }




//!
//! \brief sd3dModelFlatHole Builds model of flat surface with hole. Outer of surface and hole must contains equals count of edges
//! \param outer             Outer of surface
//! \param hole              Hole
//! \param color             Color of model
//! \return                  Model with hole
//!
Sd3dModel sd3dModelFlatHole(const Sd3dRegion &outer, const Sd3dRegion &hole, QColor color)
  {
  int outerCount = outer.count();
  int holeCount = hole.count();
  Sd3dModel model;
  if( outerCount == holeCount ) {
    //Make as equivalent
    Sd3dFace face;
    face.mColor = color;
    face.mContour.reserve(4);
    for( int i = 0; i < outerCount; i++ ) {
      QVector3D v0 = outer.at(i);
      QVector3D v1 = i + 1 < outerCount ? outer.at( i + 1 ) : outer.first();
      QVector3D v2 = i + 1 < holeCount ? hole.at( i + 1 ) : hole.first();
      QVector3D v3 = hole.at(i);
      face.mContour.clear();
      face.mContour << v0 << v1 << v2 << v3;
      model.append( face );
      }
    }
  return model;
  }




//!
//! \brief sd3dModelBox Builds box model from its size
//! \param lenght       Lenght of box (x)
//! \param width        Width of box (y)
//! \param height       Height of box (z)
//! \param color        Color faces of box
//! \return             Box model
//!
Sd3dModel sd3dModelBox(float lenght, float width, float height, QColor color)
  {
  return sd3dModelExtrude( sd3dRegionRectangle( lenght, width ), height, color );
  }




//!
//! \brief sd3dModelVolume Append model volume to volume matrix
//! \param model           Model which volume appended
//! \param volume          Source and destignation volume
//!
void sd3dModelVolume( const Sd3dModel &model, QMatrix2x3 &volume)
  {
  for( auto const &face : model )
    face.volume( volume );
  }




Sd3dModel sd3dModelPinTqfp(float width, float thickness, float fullLenght, float plateLenght, float height, QColor color )
  {
  float curveRadius = 2.0 * thickness;
  float plateFlatLenght = plateLenght - curveRadius;

  Sd3dModel pin = sd3dModelBox( thickness, width, plateFlatLenght, color );
  Sd3dFace top = pin.takeLast();
  pin += sd3dModelCurve( top.mContour, QVector3D( -curveRadius, 0, -plateFlatLenght ), QVector3D( 0, 1, 0), 90, 30, color, false );

  top = pin.takeLast();
  Sd3dModel a = sd3dModelExtrude( top.mContour, height - 2 * curveRadius, color );
  a.removeFirst();
  pin += a;

  top = pin.takeLast();
  pin += sd3dModelCurve( top.mContour, QVector3D( -height + curveRadius, 0, -plateFlatLenght - 2 * curveRadius ), QVector3D( 0, 1, 0), -90, 30, color, false );

  top = pin.takeLast();
  a = sd3dModelExtrude( top.mContour, (fullLenght - plateFlatLenght - 2 * curveRadius), color );
  a.removeFirst();
  pin += a;

  QVector3D origin = sd3dVertexCenterOfRegion( pin.last().mContour );
  QMatrix4x4 matrix;
  matrix.rotate( -90, QVector3D( 0, 1, 0) );
  matrix.translate( -origin );
  sd3dModelMapInPlace( pin, matrix );

  return pin;
  }








void Sd3dModel::clear()
  {
  mVertexList.clear();
  mInstanceList.clear();
  }



Sd3dRegion Sd3dModel::vertexList(const QList<int> &indexList) const
  {
  Sd3dRegion region;
  for( auto index : indexList )
    region.append( vertex(index) );
  return region;
  }




QMatrix4x4 Sd3dModel::matrixShift(const Sd3drFace &face, float shift)
  {
  QMatrix4x4 map;
  if( face.count() < 3 )
    return map;
  //Normal vector
  QVector3D normal = QVector3D::normal( vertex( face.at(0) ), vertex( face.at(1) ), vertex( face.at(2) ) );
  normal *= -shift;
  map.translate( normal );
  return map;
  }




Sd3drFace Sd3dModel::faceFromRegion(Sd3dRegion r)
  {
  Sd3drFace regionRef;
  for( auto v : r ) {
    int vertexIndex = vertexAppend( v );
    regionRef.append(vertexIndex);
    }
  return regionRef;
  }




Sd3drFace Sd3dModel::faceFlat(const QList<float> &pairList, int orientation)
  {
  Sd3drFace face;
  float xy( orientation == 0 ? 1.0 : 0 );
  float yz( orientation == 1 ? 1.0 : 0 );
  float xz( orientation == 2 ? 1.0 : 0 );
  int count = pairList.count() / 2;
  if( count < 3 )
    return face;
  count <<= 1;
  QVector3D v;
  for( int i = 0; i < count; i += 2 ) {
    v += QVector3D( pairList.at(i) * xy + pairList.at(i) * xz, pairList.at(i+1) * xy + pairList.at(i) * yz, pairList.at(i+1) * yz + pairList.at(i+1) * xz );
    face.append( vertexAppend(v) );
    }
  return face;
  }



Sd3drFace Sd3dModel::faceCircle(float radius, float stepDegree, const QMatrix4x4 &map)
  {
  Sd3drFace region;
  //Build circle with step 10 degree
  for( float angleDegree = 0; angleDegree < 360.0; angleDegree += stepDegree ) {
    //Convert degree to radians
    float angle = angleDegree * M_PI / 180.0;
    //Build next corner
    QVector3D v( sin(angle) * radius, cos(angle) * radius, 0 );
    //Append corner to region
    region.append( vertexAppend( map.map(v) )  );
    }
  return region;
  }



Sd3drFace Sd3dModel::faceCircleSide(float radius, int sideCount, const QMatrix4x4 &map)
  {
  float stepDegree = 360.0 / sideCount;
  return faceCircle( radius, stepDegree, map );
  }



Sd3drFace Sd3dModel::faceRectangle(float lenght, float width, const QMatrix4x4 &map)
  {
  width /= 2.0;
  lenght /= 2.0;
  QVector3D p0( -lenght, -width, 0 );
  QVector3D p1(  lenght, -width, 0 );
  QVector3D p2(  lenght,  width, 0 );
  QVector3D p3( -lenght,  width, 0 );

  return Sd3drFace( { vertexAppend( map.map(p0) ), vertexAppend( map.map(p1)), vertexAppend( map.map(p2) ), vertexAppend( map.map(p3) ) } );
  }




Sd3drFace Sd3dModel::faceRectangleSide(float width, float lenght, int sideCount, const QMatrix4x4 &map)
  {
  float edgeOnSide = sideCount / 4;
  float stepw = width / edgeOnSide;
  float steph = lenght / edgeOnSide;
  QVector3D v( width/2.0, lenght/2.0, 0);
  Sd3drFace face;
  face.append( vertexAppend( map.map( v ) )   );
  for( int i = 0; i < edgeOnSide / 2; i++ ) {
    v += QVector3D( stepw, 0, 0 );
    face.append( vertexAppend( map.map( v ) )   );
    }
  for( int i = 0; i < edgeOnSide; i++ ) {
    v += QVector3D( 0, -steph, 0 );
    face.append( vertexAppend( map.map( v ) )   );
    }
  for( int i = 0; i < edgeOnSide; i++ ) {
    v += QVector3D( -stepw, 0, 0 );
    face.append( vertexAppend( map.map( v ) )   );
    }
  for( int i = 0; i < edgeOnSide; i++ ) {
    v += QVector3D( 0, steph, 0 );
    face.append( vertexAppend( map.map( v ) )   );
    }
  for( int i = 0; i < edgeOnSide / 2; i++ ) {
    v += QVector3D( stepw, 0, 0 );
    face.append( vertexAppend( map.map( v ) )   );
    }
  return face;
  }




Sd3drFace Sd3dModel::faceDuplicate(const Sd3drFace &face, const QMatrix4x4 &map)
  {
  Sd3drFace dest;
  dest.reserve(face.count());
  for( auto index : face )
    dest.append( vertexAppend( map.map(vertex(index)) )  );
  return dest;
  }





Sd3drFace Sd3dModel::faceTrapezoidZ(const Sd3drFace &face, float height, float lessX, float lessY, float topShiftX, float topShiftY)
  {
  float x(0), y(0);
  for( auto index : face ) {
    QVector3D v = vertex( index );
    x = qMax( x, qAbs(v.x()) );
    y = qMax( y, qAbs(v.y()) );
    }
  float topX = x - lessX;
  float topY = y - lessY;
  if( x == 0 || y == 0 )
    return Sd3drFace{};
  QMatrix4x4 map;
  map.scale( topX / x, topY / y, 1.0 );
  map.translate( topShiftX, topShiftY, height );
  return faceDuplicate( face, map );
  }







Sd3drFaceList Sd3dModel::faceListWall(const Sd3drFace &face1, const Sd3drFace &face2, bool close)
  {
  Sd3drFaceList walls;
  if( face1.count() != face2.count() || face1.count() < 3 )
    return walls;

  for( int i = 0; i < face1.count() - 1; i++ ) {
    Sd3drFace wall( { face1.at(i), face1.at(i+1), face2.at(i+1), face2.at(i) } );
    walls.append( wall );
    }
  if( close ) {
    //Append wall with n-1 and 0 vertex
    Sd3drFace wall( { face1.last(), face1.first(), face2.first(), face2.last() } );
    walls.append( wall );
    }

  return walls;
  }







Sd3drFaceList Sd3dModel::faceListExtrude(const Sd3drFace &face, const QMatrix4x4 &map)
  {
  Sd3drFaceList faceList;
  //Append bottom
  faceList.append( face );

  //Create top
  Sd3drFace other = faceDuplicate( face, map );

  //Append walls
  faceList.append( faceListWall( face, other, true ) );

  //Append top
  faceList.append( other );

  return faceList;
  }




Sd3drFaceList Sd3dModel::faceListExtrudeShift(const Sd3drFace &face, float shift)
  {
  return faceListExtrude( face, matrixShift( face, shift )   );
  }



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
Sd3drFaceList Sd3dModel::faceListBodyBeveled(float bodyLenght, float bodyWidth, float bodyHeight, float bevelFront, float bevelSide, float verticalHeight )
  {
  //Bottom rectangle
  Sd3drFace bottom = faceRectangle( bodyLenght, bodyWidth, QMatrix4x4() );

  float middleHeight = (bodyHeight - verticalHeight) / 2.0;
  Sd3drFace middleLow = faceTrapezoidZ( bottom, middleHeight, -bevelSide, -bevelSide, 0, 0 );

  Sd3drFace middleHigh = faceDuplicateShift( middleLow, verticalHeight );

  float deltaBevel = (bevelFront - bevelSide) / 2.0;
  Sd3drFace top = faceTrapezoidZ( bottom, bodyHeight, 0, deltaBevel, 0, deltaBevel );

  //Build walls
  Sd3drFaceList body;
  body.append( bottom );
  body.append( faceListWall( bottom, middleLow, true ) );
  body.append( faceListWall( middleLow, middleHigh, true ) );
  body.append( faceListWall( middleHigh, top, true ) );
  body.append( top );

  return body;
  }




Sd3drFaceList Sd3dModel::faceListBox(float bodyLenght, float bodyWidth, float bodyHeight)
  {
  return faceListExtrudeShift( faceRectangle( bodyLenght, bodyWidth, QMatrix4x4() ), bodyHeight );
  }



Sd3drFaceList Sd3dModel::faceListCylinder(float radius, float height)
  {
  return faceListExtrudeShift( faceCircleSide( radius, qMax( 20, static_cast<int>(radius * 6.28) ), QMatrix4x4() ), height );
  }



//!
//! \brief sd3dModelHexagon Builds hexagonal box body of part
//! \param lenght           Lenght of box
//! \param topLenght        Top lenght excluding bevels
//! \param height           Height of box
//! \param width            Width of box
//! \param color            Color of faces of model
//! \return                 Model of hexagonal box body of part
//!
Sd3drFaceList Sd3dModel::faceListHexagon(float lenght, float topLenght, float height, float width)
  {
  //Profile of hexagon
  float bevelLenght = (lenght - topLenght) / 2.0;
  QVector3D v0( -topLenght / 2.0, -width / 2.0, 0 );
  QVector3D v1 = v0 + QVector3D( -bevelLenght, 0, height / 2.0 );
  QVector3D v2 = v1 + QVector3D(  bevelLenght, 0, height / 2.0 );
  QVector3D v3 = v2 + QVector3D(  topLenght, 0, 0 );
  QVector3D v4 = v3 + QVector3D(  bevelLenght, 0, -height / 2.0 );
  QVector3D v5 = v4 + QVector3D( -bevelLenght, 0, -height / 2.0 );
  Sd3drFace faceLeft;
  faceLeft << vertexAppend(v0) << vertexAppend(v1) << vertexAppend(v2) << vertexAppend(v3) << vertexAppend(v4) << vertexAppend(v5);
  return faceListExtrudeShift( faceLeft, width );
  }








//!
//! \brief json Overloaded function to write object content into json writer
//!             Overrided function
//! \param js   Json writer
//!
void Sd3dModel::json(SdJsonWriter &js) const
  {
  json3dRegion( js, QStringLiteral("Vertex"), mVertexList );
  js.jsonList( js, QStringLiteral("Instance"), mInstanceList );
  }





//!
//! \brief json Overloaded function to read object content from json reader
//!             Overrided function
//! \param js   Json reader
//!
void Sd3dModel::json(const SdJsonReader &js)
  {
  json3dRegion( js, QStringLiteral("Vertex"), mVertexList );
  js.jsonList( js, QStringLiteral("Instance"), mInstanceList );
  }




void Sd3dModel::draw3d(QOpenGLFunctions_2_0 *f) const
  {
  for( auto const &instance : mInstanceList )
    instance.draw( f, mVertexList );
  }
