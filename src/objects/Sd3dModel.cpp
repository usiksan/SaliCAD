#include "Sd3dModel.h"
#include "script/SdScriptParser3d.h"
#include "SdJsonIO.h"

#include <math.h>




#if 0



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


#endif





Sd3ColorList Sd3dModel::colorList() const
  {
  Sd3ColorList list;
  for( auto const &inst : mInstanceList )
    inst.bodyColorGet( list );
  return list;
  }



void Sd3dModel::colorListSet(const Sd3ColorList &lst)
  {
  int index = 0;
  for( int i = 0; i < mInstanceList.count(); i++ )
    mInstanceList[i].bodyColorSet( lst, index );
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




Sd3drFace Sd3dModel::faceFlat( int firstVertexIndex, const QList<float> &pairList, int orientation)
  {
  Sd3drFace face;
  float xy( orientation == 0 ? 1.0 : 0 );
  float yz( orientation == 1 ? 1.0 : 0 );
  float xz( orientation == 2 ? 1.0 : 0 );
  int count = pairList.count() / 2;
  if( count < 2 )
    return face;
  count <<= 1;
  QVector3D v( vertex(firstVertexIndex) );
  face.append(firstVertexIndex);
  for( int i = 0; i < count; i += 2 ) {
    QVector3D dv( pairList.at(i) * xy + pairList.at(i) * xz, pairList.at(i+1) * xy + pairList.at(i) * yz, pairList.at(i+1) * yz + pairList.at(i+1) * xz );
    v += dv;
    face.append( vertexAppend(v) );
    }
  return face;
  }



Sd3drFace Sd3dModel::faceCircle(float radius, float stepDegree, const QMatrix4x4 &map)
  {
  Sd3drFace region;
  //Build circle with step degree
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



Sd3drFace Sd3dModel::faceRectangleRound(float lenght, float width, float radius, float stepDegree, const QMatrix4x4 &map)
  {
  Sd3drFace region;
  float curX = lenght / 2 - radius;
  float curY = width / 2 - radius;
  //Top right corner
  for( float angleDegree = 0; angleDegree <= 90.0; angleDegree += stepDegree ) {
    //Convert degree to radians
    float angle = angleDegree * M_PI / 180.0;
    //Build next corner
    QVector3D v( sin(angle) * radius + curX, cos(angle) * radius + curY, 0 );
    //Append corner to region
    region.append( vertexAppend( map.map(v) )  );
    }
  //Right edge
  curY = -curY;
  region.append( vertexAppend( map.map(QVector3D( curX + radius, curY, 0 )) )  );

  //Bottom right corner
  for( float angleDegree = 90.0 + stepDegree; angleDegree <= 180.0; angleDegree += stepDegree ) {
    //Convert degree to radians
    float angle = angleDegree * M_PI / 180.0;
    //Build next corner
    QVector3D v( sin(angle) * radius + curX, cos(angle) * radius + curY, 0 );
    //Append corner to region
    region.append( vertexAppend( map.map(v) )  );
    }
  //Bottom edge
  curX = -curX;
  region.append( vertexAppend( map.map(QVector3D( curX, curY - radius, 0 )) )  );

  //Bottom left corner
  for( float angleDegree = 180.0 + stepDegree; angleDegree <= 270.0; angleDegree += stepDegree ) {
    //Convert degree to radians
    float angle = angleDegree * M_PI / 180.0;
    //Build next corner
    QVector3D v( sin(angle) * radius + curX, cos(angle) * radius + curY, 0 );
    //Append corner to region
    region.append( vertexAppend( map.map(v) )  );
    }
  //Left edge
  curY = -curY;
  region.append( vertexAppend( map.map(QVector3D( curX - radius, curY, 0 )) )  );

  //Top left corner
  for( float angleDegree = 270.0 + stepDegree; angleDegree <= 360.0; angleDegree += stepDegree ) {
    //Convert degree to radians
    float angle = angleDegree * M_PI / 180.0;
    //Build next corner
    QVector3D v( sin(angle) * radius + curX, cos(angle) * radius + curY, 0 );
    //Append corner to region
    region.append( vertexAppend( map.map(v) )  );
    }

  //Top edge is automatic as closed edge

  return region;
  }




Sd3drFace Sd3dModel::faceRectangleSide(float width, float lenght, int sideCount, const QMatrix4x4 &map)
  {
  float edgeOnSide = sideCount / 4;
  float stepw = width / edgeOnSide;
  float steph = lenght / edgeOnSide;
  QVector3D v( 0, lenght/2.0, 0);
  Sd3drFace face;
  //face.append( vertexAppend( map.map( v ) )   );
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

Sd3drFace Sd3dModel::faceDuplicateScale(const Sd3drFace &face, float scalex, float scaley, float shift)
  {
  QMatrix4x4 map;
  map.translate( 0, 0, shift );
  map.scale( scalex, scaley, 1.0 );
  return faceDuplicate( face, map );
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




Sd3drFace Sd3dModel::faceCurveXZ(const Sd3drFace &face, float radius, float angleSrc, float angleDst)
  {
  //Find center of face
  QVector3D center;
  for( auto const &v : face )
    center += vertex(v);
  float count = face.count();
  center = center / count;

  QVector3D rotationCenter( radius, 0.0, 0.0 );
  QMatrix4x4 rotationMap;
  rotationMap.rotate( angleSrc, 0.0, 1.0, 0.0 );
  center += rotationMap.map( rotationCenter );

  QMatrix4x4 centerMap;
  //Reverse order of transformations
  centerMap.translate( center );
//  centerMap.translate( radius, 0 );
  centerMap.rotate( angleDst, 0.0, 1.0, 0.0 );
//  centerMap.translate( -radius, 0 );
//  centerMap.rotate( -angleSrc, 0.0, 1.0, 0.0 );
  centerMap.translate( -center.x(), -center.y(), -center.z() );


  Sd3drFace dest;
  dest.reserve( face.count() );
  for( auto const &v : face )
    dest.append( vertexAppend( centerMap.map(vertex(v)) )  );

  return dest;
  }



//!
//! \brief faceEqudistante Calculate equidistant face of give face with distance. At last make transformation with map
//! \param face            Source face
//! \param distance        Distance of new face
//! \param map             Finish translation map
//! \return                Equidistant face
//!
Sd3drFace Sd3dModel::faceEqudistanteXY(const Sd3drFace &face, float distance, const QMatrix4x4 &map)
  {
  if( face.count() < 3 )
    return face;

  //Fill contour with segments
  QList<QLineF> eq;
  eq.reserve( face.count() );
  QLineF line;
  //First segment
  line.setP1( point(face.last()) );
  line.setP2( point(face.first()) );
  eq.append( line );
  for( int i = 1; i < face.count(); i++ ) {
    line.setP1( point(face.at(i-1)) );
    line.setP2( point(face.at(i)) );
    eq.append( line );
    }

  //For each segment calculate paralled line
  for( int i = 0; i < face.count(); i++ ) {
    line = eq.at(i);
    float dx = line.dx();
    float dy = line.dy();
    float len = line.length();
    if( len != 0 ) {
      float px = distance * dy / len;
      float py = distance * dx / len;
      eq[i].translate( px, -py );
      }
    }

  //For each segment calculate intersection point with next segment and place point into result face
  Sd3drFace faceEq;
  QPointF v;
  for( int i = 1; i < eq.count(); i++ ) {
    if( eq[i].intersects( eq.at(i-1), &v ) == QLineF::NoIntersection )
      v = eq.at(i).p1();
    faceEq.append( vertexAppend( map.map( QVector3D(v))) );
    }
  //Last segment
  if( eq.first().intersects( eq.last(), &v ) == QLineF::NoIntersection )
    v = eq.first().p1();
  faceEq.append( vertexAppend( map.map( QVector3D(v))) );
  return faceEq;
  }





Sd3drFace Sd3dModel::facePart(const Sd3drFace &face, const QList<float> &indexes)
  {
  Sd3drFace dest;
  for( auto v : indexes ) {
    int i = static_cast<int>(v);
    if( i >= 0 && i < face.count() )
      dest.append( face.at(i) );
    }
  return dest;
  }


struct SdTriangle
  {
    QPointF mLeftA;
    QPointF mNextB;
    QPointF mPrevC;

    float dyba;
    float dxba;
    float dycb;
    float dxcb;
    float dyac;
    float dxac;
    void prepare()
      {
      dyba = mNextB.y() - mLeftA.y();
      dxba = mNextB.x() - mLeftA.x();

      dycb = mPrevC.y() - mNextB.y();
      dxcb = mPrevC.x() - mNextB.x();

      dyac = mLeftA.y() - mPrevC.y();
      dxac = mLeftA.x() - mPrevC.x();
      }

    bool isPointInside( QPointF p )
      {
      float v0 = (mLeftA.x() - p.x()) * dyba - (mLeftA.y() - p.y()) * dxba;
      float v1 = (mNextB.x() - p.x()) * dycb - (mNextB.y() - p.y()) * dxcb;
      float v2 = (mPrevC.x() - p.x()) * dyac - (mPrevC.y() - p.y()) * dxac;
      return (v0 >= 0 && v1 >= 0 && v2 >= 0) || (v0 < 0 && v1 < 0 && v2 < 0);
      }
  };



struct SdPointLink
  {
    QPointF      mPoint;
    int          mIndex;
    SdPointLink *mNext;
    SdPointLink *mPrev;
  };


Sd3drFaceList Sd3dModel::faceListSimplify( const Sd3drFace &srcFace )
  {
  Sd3drFaceList faceList;
  Sd3drFaceList srcList;
  srcList.append( srcFace );
  for( int faceIndex = 0; faceIndex < srcList.count(); faceIndex++ ) {
    Sd3drFace face = srcList.at(faceIndex);
    while( face.count() > 3 ) {
      int ileft = lessLeft( face );
      int inext = next( ileft, face );
      int iprev = prev( ileft, face );
      SdTriangle t;
      t.mLeftA = point( face.at(ileft) );
      t.mNextB = point( face.at(inext) );
      t.mPrevC = point( face.at(iprev) );
      t.prepare();

      //Test all remain points of region
      QPointF p;
      int innerIndex = -1;
      for( int index = next( inext, face ); index != iprev; index = next( index, face ) ) {
        QPointF v = point( face.at(index) );
        if( t.isPointInside( v ) ) {
          if( innerIndex < 0 || isLeft( v, p ) ) {
            p = v;
            innerIndex = index;
            }
          }
        }
      if( innerIndex >= 0 ) {
        //Divide source region into two regions
        Sd3drFace faceLeft, faceRight;
        for( int index = ileft; index != innerIndex; index = next( index, face ) )
          faceLeft.append( face.at(index) );
        faceLeft.append( face.at(innerIndex) );
        for( int index = ileft; index != innerIndex; index = prev( index, face ) )
          faceRight.append( face.at(index) );
        faceRight.append( face.at(innerIndex) );
        face = faceLeft;
        srcList.append( faceRight );
        //faceList.append( faceListSimplify(faceRight) );
        }
      else {
        //Remove triangle from face
        Sd3drFace triangle;
        triangle.append( face.at(ileft) );
        triangle.append( face.at(inext) );
        triangle.append( face.at(iprev) );
        face.removeAt( ileft );
        faceList.append( triangle );
        }
      }
    faceList.append( face );
    }
  return faceList;
  }




Sd3drFaceList Sd3dModel::faceListHoles(const Sd3drFace &srcFace, Sd3drFaceList &holeList)
  {
  Sd3drFaceList faceList;
  Sd3drFaceList srcList;
  srcList.append( srcFace );
  for( int faceIndex = 0; faceIndex < srcList.count(); faceIndex++ ) {
    Sd3drFace face = srcList.at(faceIndex);
    while( face.count() > 3 ) {
      int ileft = lessLeft( face );
      int inext = next( ileft, face );
      int iprev = prev( ileft, face );
      SdTriangle t;
      t.mLeftA = point( face.at(ileft) );
      t.mNextB = point( face.at(inext) );
      t.mPrevC = point( face.at(iprev) );
      t.prepare();

      //Test all remain points of region and holes
      QPointF p;
      int innerIndex = -1;
      int innerFace = -1;
      //Test all remain points of region
      for( int index = next( inext, face ); index != iprev; index = next( index, face ) ) {
        QPointF v = point( face.at(index) );
        if( t.isPointInside( v ) ) {
          if( innerIndex < 0 || isLeft( v, p ) ) {
            p = v;
            innerIndex = index;
            }
          }
        }
      //Test all holes
      for( int holeIndex = 0; holeIndex < holeList.count(); holeIndex++ ) {
        auto const &hole = holeList.at(holeIndex);
        for( int index = 0; index < hole.count(); index++ ) {
          QPointF v = point( hole.at(index) );
          if( t.isPointInside( v ) ) {
            if( innerIndex < 0 || isLeft( v, p ) ) {
              p = v;
              innerIndex = index;
              innerFace = holeIndex;
              }
            }
          }
        }
      if( innerIndex >= 0 ) {
        if( innerFace < 0 ) {
          //Inner point is from current region
          //Divide source region into two regions
          Sd3drFace faceLeft, faceRight;
          for( int index = ileft; index != innerIndex; index = next( index, face ) )
            faceLeft.append( face.at(index) );
          faceLeft.append( face.at(innerIndex) );
          for( int index = ileft; index != innerIndex; index = prev( index, face ) )
            faceRight.append( face.at(index) );
          faceRight.append( face.at(innerIndex) );
          face = faceLeft;
          srcList.append( faceRight );
          }
        else {
          //Hole need to be added
          Sd3drFace unionFace;
          for( int index = ileft; )
          }
        }
      else {
        //Remove triangle from face
        Sd3drFace triangle;
        triangle.append( face.at(ileft) );
        triangle.append( face.at(inext) );
        triangle.append( face.at(iprev) );
        face.removeAt( ileft );
        faceList.append( triangle );
        }
      }
    faceList.append( face );
    }
  return faceList;
  }







//!
//! \brief faceListWalls  Builds walls on base bottom and top regions. Walls builded with color
//! \param face1          First bound of walls - Bottom
//! \param face2          Second bound of walls - Top
//! \param close          If true then append wall with n-1 and 0 index vertex
//! \return               List of walls
//!
Sd3drFaceList Sd3dModel::faceListWall(const Sd3drFace &face1, const Sd3drFace &face2, bool close)
  {
  Sd3drFaceList walls;
  if( face1.count() != face2.count() || face1.count() < 2 )
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




Sd3drFaceList Sd3dModel::faceListWallIndexed(const Sd3drFace &face1, const Sd3drFace &face2, const QList<float> &indexes)
  {
  Sd3drFaceList walls;

  for( auto v : indexes ) {
    int i = static_cast<int>(v);
    if( i < face1.count() && i < face2.count() && i >= 0 ) {
      Sd3drFace wall( { face1.at(i), (i+1 < face1.count()) ? face1.at(i+1) : face1.first(), (i+1 < face2.count()) ? face2.at(i+1) : face2.first(), face2.at(i) } );
      walls.append( wall );
      }
    }

  return walls;
  }




Sd3drFaceList Sd3dModel::faceListIndexed(const Sd3drFaceList &faceList, const QList<float> &indexes)
  {
  Sd3drFaceList walls;

  for( auto v : indexes ) {
    int i = static_cast<int>(v);
    if( i < faceList.count() && i >= 0 )
      walls.append( faceList.at(i) );
    }

  return walls;
  }







//!
//! \brief faceListExtrude Extrudes model from source face in direction specified by map
//! \param face            Source face of model
//! \param map             Direction of extruding
//! \return                Solid model: floor, roof and walls
//!
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




//!
//! \brief faceListExtrudeShift Extrudes model from region in the direction of the normal vector with
//!                             specified shift amount color faces.
//!                             First face is bottom (begin of vector), last face is top (end of vector),
//!                             Middle faces are walls.
//! \param face                 Region of bottom of model
//! \param shift                Shift amount of extrude
//! \return                     Solid model extruded from region in the direction of the normal vector
//!
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




//!
//! \brief faceListBox  Builds box model from its size
//! \param lenght       Lenght of box (x)
//! \param width        Width of box (y)
//! \param height       Height of box (z)
//! \param color        Color faces of box
//! \return             Box model
//!
Sd3drFaceList Sd3dModel::faceListBox(float lenght, float width, float height)
  {
  return faceListExtrudeShift( faceRectangle( lenght, width, QMatrix4x4() ), height );
  }



//!
//! \brief faceListCylinder  Builds cylinder model from its size
//! \param radius            Radius of circle of footing of cylinder
//! \param height            Height of cylinder
//! \return                  Cylinder model
//!
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




Sd3drFaceList Sd3dModel::faceListCurveXZ(const Sd3drFace &face, float radius, float angleSrc, float angleDst, int sideCount, bool close )
  {
  Sd3drFaceList model;
  Sd3drFace first(face);
  Sd3drFace second;
  float angleStep = angleDst / sideCount;
  float angle = angleStep;
  for( int i = 1; i < sideCount; i++ ) {
    second = faceCurveXZ( face, radius, angleSrc, angle );
    model.append( faceListWall( first, second, close ) );
    angle += angleStep;
    first = second;
    }
  //Last walls
  second = faceCurveXZ( face, radius, angleSrc, angleDst );
  model.append( faceListWall( first, second, close ) );
  return model;
  }




Sd3drFaceList Sd3dModel::faceListPinCurveOne(const Sd3drFace &face, float firstLen, float middleLen, float radius, float angle, int sideCount)
  {
  Sd3drFaceList pin;
  Sd3drFace faceMiddleStart = faceDuplicateShift( face, firstLen );
  pin = faceListWall( face, faceMiddleStart, true );
  Sd3drFace faceMiddleStop = faceCurveXZ( faceMiddleStart, radius, 0, angle );
  pin.append( faceListCurveXZ( faceMiddleStart, radius, 0, angle, sideCount, true ) );
  Sd3drFace faceLast = faceDuplicateShift( faceMiddleStop, middleLen );
  pin.append( faceListWall( faceMiddleStop, faceLast, true ) );
  pin.append( faceLast );
  return pin;
  }




Sd3drFaceList Sd3dModel::faceListPinCurveTwo(const Sd3drFace &face, float firstLen, float middleLen, float lastLen, float radius, float angleFirst, float angleSecond, int sideCount)
  {
  Sd3drFaceList pin;
  Sd3drFace faceMiddleStart = faceDuplicateShift( face, firstLen );
  pin = faceListWall( face, faceMiddleStart, true );
  Sd3drFace faceMiddleStop = faceCurveXZ( faceMiddleStart, radius, 0, angleFirst );
  pin.append( faceListCurveXZ( faceMiddleStart, radius, 0, angleFirst, sideCount, true ) );
  Sd3drFace faceSecondStart = faceDuplicateShift( faceMiddleStop, middleLen );
  pin.append( faceListWall( faceMiddleStop, faceSecondStart, true ) );
  Sd3drFace faceSecondStop = faceCurveXZ( faceSecondStart, -radius, angleFirst, angleSecond );
  pin.append( faceListCurveXZ( faceSecondStart, -radius, angleFirst, angleSecond, sideCount, true ) );
  Sd3drFace faceLast = faceDuplicateShift( faceSecondStop, lastLen );
  pin.append( faceListWall( faceSecondStop, faceLast, true ) );
  pin.append( faceLast );
  return pin;
  }




Sd3drFaceList Sd3dModel::faceListPinTqfp(float width, float thickness, float fullLenght, float plateLenght, float height)
  {
  return faceListPinCurveTwo( faceRectangle( width, thickness, QMatrix4x4()), fullLenght - plateLenght, height, plateLenght, thickness, 90, -90, 30 );
  }




Sd3drFaceList Sd3dModel::faceListWallRound(const Sd3drFace &face1, const Sd3drFace &face2, float scaleX, float scaleY, float radius, float stepDegree)
  {
  if( stepDegree >= 90.0 )
    return faceListWall( face1, face2, true );

  Sd3drFaceList faceList;
  Sd3drFace first = face1;
  float scaleDX = scaleX - 1.0;
  float scaleDY = scaleY - 1.0;
  if( stepDegree < 0 ) {
    stepDegree = -stepDegree;
    for( float angleDegree = stepDegree; angleDegree < 90.0; angleDegree += stepDegree ) {
      //Convert degree to radians
      float angle = angleDegree * M_PI / 180.0;
      Sd3drFace second = faceDuplicateScale( face1, 1.0 + scaleDX * sin(angle), 1.0 + scaleDY * sin(angle), radius - radius * cos(angle) );
      faceList.append( faceListWall( first, second, true ) );
      first = second;
      }
    }
  else {
    for( float angleDegree = stepDegree; angleDegree < 90.0; angleDegree += stepDegree ) {
      //Convert degree to radians
      float angle = angleDegree * M_PI / 180.0;
      Sd3drFace second = faceDuplicateScale( face1, 1.0 + scaleDX * (1.0 - cos(angle)), 1.0 + scaleDY * (1.0 - cos(angle)), radius * sin(angle) );
      faceList.append( faceListWall( first, second, true ) );
      first = second;
      }
    }
  faceList.append( faceListWall( first, face2, true ) );
  return faceList;
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



void Sd3dModel::build(const QString &script)
  {
  clear();
  SdScriptParser3d parser( nullptr, this );
  auto prog = parser.parse3d( script, nullptr, this );
  if( parser.error().isEmpty() )
    prog->execute();
  }




//!
//! \brief volumeAdd Append volume of model to result volume
//! \param volume    Source and result volume
//!
void Sd3dModel::volumeAdd(QMatrix2x3 &volume) const
  {
  for( auto const &inst : qAsConst(mInstanceList) ) {
    inst.volumeAdd( volume, mVertexList );
    }
  }


int Sd3dModel::lessLeft(const Sd3drFace &face) const
  {
  //Scan region and find most less point
  int less = 0;
  QPointF p = point(face.at(0));
  for( int i = 1; i < face.count(); i++ ) {
    QPointF v = point( face.at(i) );
    if( isLeft( v, p ) ) {
      p = v;
      less = i;
      }
    }
  return less;
  }

bool Sd3dModel::isLeft(QPointF p1, QPointF p2) const
  {
  return p1.x() < p2.x() || ((p1.x() == p2.x()) && (p1.y() < p2.y()));
  }


int Sd3dModel::next(int center, const Sd3drFace &face) const
  {
  if( center + 1 < face.count() )
    return center + 1;
  return 0;
  }

int Sd3dModel::prev(int center, const Sd3drFace &face) const
  {
  if( center > 0 )
    return center - 1;
  return face.count() - 1;
  }
