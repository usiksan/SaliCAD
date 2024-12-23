#include "Sd3drModel.h"
#include "SvLib/SvJsonIO.h"
#include "Sd3drPointLink.h"

#include <QLineF>
#include <math.h>





Sd3ColorList Sd3drModel::colorList() const
  {
  Sd3ColorList list;
  for( auto const &inst : mInstanceList )
    inst.bodyColorGet( list );
  return list;
  }



void Sd3drModel::colorListSet(const Sd3ColorList &lst)
  {
  int index = 0;
  for( int i = 0; i < mInstanceList.count(); i++ )
    mInstanceList[i].bodyColorSet( lst, index );
  }



void Sd3drModel::clear()
  {
  mVertexList.clear();
  mInstanceList.clear();
  }



Sd3drRegion Sd3drModel::vertexList(const QList<int> &indexList) const
  {
  Sd3drRegion region;
  for( auto index : indexList )
    region.append( vertex(index) );
  return region;
  }




QMatrix4x4 Sd3drModel::matrixShift(const Sd3drFace &face, float shift)
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




Sd3drFace Sd3drModel::faceFromRegion(Sd3drRegion r)
  {
  Sd3drFace regionRef;
  for( auto v : r ) {
    int vertexIndex = vertexAppend( v );
    regionRef.append(vertexIndex);
    }
  return regionRef;
  }




//!
//! \brief faceFlat         Builds flat face from list of pairs floats
//! \param firstVertexIndex First vertex index from which builds face
//! \param pairList         Pair list as offset from current point begining from firstVertexIndex
//! \param orientation      0 - xy, 1 - yz, 2 - xz
//! \return
//!
Sd3drFace Sd3drModel::faceFlat( int firstVertexIndex, const QList<float> &pairList, int orientation)
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



//!
//! \brief faceFlatMatrix Builds flat face from list of pairs float. First pair is source of path and next pairs are offsets from previous pair
//! \param pairList       List of pairs float. First pair is source of path and next pairs are offsets from previous pair
//! \param map            Transfer map
//! \return               Face
//!
Sd3drFace Sd3drModel::faceFlatMatrix(const QList<float> &pairList, const QMatrix4x4 &map)
  {
  Sd3drFace face;
  int count = pairList.count() / 2;
  if( count < 3 )
    return face;
  count <<= 1;
  QVector3D v( pairList.at(0), pairList.at(1), 0 );
  face.append( vertexAppend( map.map(v) ) );
  for( int i = 2; i < count; i += 2 ) {
    QVector3D dv( pairList.at(i), pairList.at(i+1), 0 );
    v += dv;
    face.append( vertexAppend( map.map(v) ) );
    }
  return face;
  }



//!
//! \brief faceCircleThis function builds circle region on base radius with center at 0
//! \param radius       Radius of builded circle
//! \param stepDegree   Step with which need to create multicorner circle region
//! \param map          Finish circle transformation map
//! \return             Circle
//!
Sd3drFace Sd3drModel::faceCircle(float radius, float stepDegree, const QMatrix4x4 &map)
  {
  return faceEllipse( radius, radius, stepDegree, map );
  }



//!
//! \brief faceCircleSide Builds circle region interpolated polygon on base radius with center at 0 and sideCount sides
//! \param radius         Radius of builded circle
//! \param sideCount      Side count of polygon
//! \param map            Finish circle transformation map
//! \return               Circle
//!
Sd3drFace Sd3drModel::faceCircleSide(float radius, int sideCount, const QMatrix4x4 &map)
  {
  float stepDegree = 360.0 / sideCount;
  return faceCircle( radius, stepDegree, map );
  }




Sd3drFace Sd3drModel::faceEllipse(float radiusx, float radiusy, float stepDegree, const QMatrix4x4 &map)
  {
  Sd3drFace region;
  //Build circle with step degree
  for( float angleDegree = 0; angleDegree < 360.0; angleDegree += stepDegree ) {
    //Convert degree to radians
    float angle = angleDegree * M_PI / 180.0;
    //Build next corner
    QVector3D v( sin(angle) * radiusx, cos(angle) * radiusy, 0 );
    //Append corner to region
    region.append( vertexAppend( map.map(v) )  );
    }
  return region;
  }




Sd3drFace Sd3drModel::faceEllipseSide(float radiusx, float radiusy, int sideCount, const QMatrix4x4 &map)
  {
  float stepDegree = 360.0 / sideCount;
  return faceEllipse( radiusx, radiusy, stepDegree, map );
  }



//!
//! \brief faceRectangle Builds rectangle region with center at 0 and four edges
//! \param lenght        Lenght of rectangle (X)
//! \param width         Width of rectangle (Y)
//! \param map           Finish rectangle transformation map
//! \return              Rectangle
//!
Sd3drFace Sd3drModel::faceRectangle(float lenght, float width, const QMatrix4x4 &map)
  {
  width /= 2.0;
  lenght /= 2.0;
  QVector3D p0( -lenght, -width, 0 );
  QVector3D p1(  lenght, -width, 0 );
  QVector3D p2(  lenght,  width, 0 );
  QVector3D p3( -lenght,  width, 0 );

  return Sd3drFace( { vertexAppend( map.map(p0) ), vertexAppend( map.map(p1)), vertexAppend( map.map(p2) ), vertexAppend( map.map(p3) ) } );
  }



Sd3drFace Sd3drModel::faceRectangleRound(float lenght, float width, float radius, float stepDegree, const QMatrix4x4 &map)
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




//!
//! \brief faceRectangleSide Builds rectangle region with center at 0 and sideCount sides reorganized to rectangle
//! \param width             Lenght of rectangle (X)
//! \param lenght            Width of rectangle (Y)
//! \param sideCount         Side count of polygon
//! \param map               Finish rectangle transformation map
//! \return                  Rectangle
//!
Sd3drFace Sd3drModel::faceRectangleSide(float lenght, float width, int sideCount, const QMatrix4x4 &map)
  {
  float edgeOnSide = sideCount / 4;
  float stepw = lenght / edgeOnSide;
  float steph = width / edgeOnSide;
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




Sd3drFace Sd3drModel::faceDuplicate(const Sd3drFace &face, const QMatrix4x4 &map)
  {
  Sd3drFace dest;
  dest.reserve(face.count());
  for( auto index : face )
    dest.append( vertexAppend( map.map(vertex(index)) )  );
  return dest;
  }




Sd3drFace Sd3drModel::faceDuplicateScale(const Sd3drFace &face, float scalex, float scaley, float shift)
  {
  QMatrix4x4 map;
  map.translate( 0, 0, shift );
  map.scale( scalex, scaley, 1.0 );
  return faceDuplicate( face, map );
  }




Sd3drFace Sd3drModel::faceDuplicateGrow(const Sd3drFace &face, float deltax, float deltay, const QMatrix4x4 &map)
  {
  //Calculate lenght (x) and width (y) of face
  //Calculate dx and dy scale
  if( face.count() < 3 )
    return face;
  //Calculate scale factor
  float sizex,sizey;
  faceSizeXY( face, sizex, sizey );
  QMatrix4x4 mp(map);
  mp.scale( (sizex + deltax) / sizex, (sizey + deltay) / sizey, 1.0 );
  return faceDuplicate( face, mp );
  }





Sd3drFace Sd3drModel::faceTrapezoidZ(const Sd3drFace &face, float height, float lessX, float lessY, float topShiftX, float topShiftY)
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




Sd3drFace Sd3drModel::faceCurveXZ(const Sd3drFace &face, float radius, float angleSrc, float angleDst)
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
Sd3drFace Sd3drModel::faceEqudistanteXY(const Sd3drFace &face, float distance, const QMatrix4x4 &map)
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




//!
//! \brief faceBevelXY Calculate finish round face of given face with radius and with shift with given direction
//! \param face        Source face
//! \param radius      Distance of new face
//! \return            Shifted round face
//!
Sd3drFace Sd3drModel::faceBevelXY(const Sd3drFace &face, float radius )
  {
  //Calculate dx and dy scale
  if( face.count() < 3 )
    return face;
  //Calculate scale factor
  float sizex,sizey;
  faceSizeXY( face, sizex, sizey );
  return faceDuplicateScale( face, (sizex + radius) / sizex, (sizey + radius) / sizey, fabs(radius) );
  }





Sd3drFace Sd3drModel::facePart(const Sd3drFace &face, const QList<float> &indexes)
  {
  Sd3drFace dest;
  for( auto v : indexes ) {
    int i = static_cast<int>(v);
    if( i >= 0 && i < face.count() )
      dest.append( face.at(i) );
    }
  return dest;
  }



Sd3drFaceList Sd3drModel::faceListDuplicate(const Sd3drFaceList &faceList, const QMatrix4x4 &map)
  {
  Sd3drFaceList dest;
  for( auto const &face : faceList )
    dest.append( faceDuplicate( face, map ) );
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




Sd3drFaceList Sd3drModel::faceListSimplifyXY( const Sd3drFace &srcFace )
  {
  Sd3drFaceList faceList;
  Sd3drPointLinkList pointPool;
  QList<Sd3drPointLinkPtr> srcList;
  srcList.append( pointPool.addRegion( this, srcFace, false ) );
  for( int faceIndex = 0; faceIndex < srcList.count(); faceIndex++ ) {
    Sd3drPointLinkPtr face = srcList.at(faceIndex);
    while( !face->isTriangle() ) {
      face = face->lessLeft();
      SdTriangle t;
      t.mLeftA = face->mPoint;
      t.mNextB = face->mNext->mPoint;
      t.mPrevC = face->mPrev->mPoint;
      t.prepare();

      //Test all remain points of region
      Sd3drPointLinkPtr inner = nullptr;
      for( Sd3drPointLinkPtr ptr = face->mNext->mNext; ptr != face->mPrev; ptr = ptr->mNext ) {
        if( t.isPointInside( ptr->mPoint ) ) {
          if( inner == nullptr || ptr->isLeft( inner ) )
            inner = ptr;
          }
        }
      if( inner != nullptr ) {
        //Divide source region into two regions
        face->splitRegion( inner, &pointPool );
        srcList.append( inner );
        }
      else {
        //Remove triangle from face
        faceList.append( face->triangle() );
        face = face->remove();
        }
      }
    faceList.append( face->triangle() );
    }
  return faceList;
  }




Sd3drFaceList Sd3drModel::faceListHolesXY(const Sd3drFace &srcFace, const Sd3drFaceList &holeList)
  {
  Sd3drFaceList faceList;
  Sd3drPointLinkList pointPool;
  QList<Sd3drPointLinkPtr> srcList;
  QList<Sd3drPointLinkPtr> srcHole;
  srcList.append( pointPool.addRegion( this, srcFace, true ) );
  for( auto const &hole : holeList )
    srcHole.append( pointPool.addRegion( this, hole, false ) );
  for( int faceIndex = 0; faceIndex < srcList.count(); faceIndex++ ) {
    Sd3drPointLinkPtr face = srcList.at(faceIndex);
    while( !face->isTriangle() ) {
      face = face->lessLeft();
      SdTriangle t;
      t.mLeftA = face->mPoint;
      t.mNextB = face->mNext->mPoint;
      t.mPrevC = face->mPrev->mPoint;
      t.prepare();

      //Test all remain points of region and holes
      Sd3drPointLinkPtr inner = nullptr;
      for( Sd3drPointLinkPtr ptr = face->mNext->mNext; ptr != face->mPrev; ptr = ptr->mNext ) {
        if( t.isPointInside( ptr->mPoint ) ) {
          if( inner == nullptr || ptr->isLeft( inner ) )
            inner = ptr;
          }
        }
      //Test holes
      int usedHole = -1;
      for( int holeIndex = 0; holeIndex < srcHole.count(); holeIndex++ ) {
        //Test one hole
        Sd3drPointLinkPtr hole = srcHole.at(holeIndex);
        Sd3drPointLinkPtr ptr = hole;
        do {
          if( t.isPointInside( ptr->mPoint ) ) {
            if( inner == nullptr || ptr->isLeft( inner ) ) {
              inner = ptr;
              usedHole = holeIndex;
              }
            }
          ptr = ptr->mNext;
          }
        while( ptr != hole );
        }
      if( inner != nullptr ) {
        //Divide source region into two regions or union source with hole
        face->splitRegion( inner, &pointPool );
        if( usedHole >= 0 )
          srcHole.removeAt(usedHole);
        else
          srcList.append( inner );
        }
      else {
        //Remove triangle from face
        faceList.append( face->triangle() );
        face = face->remove();
        }
      }
    faceList.append( face->triangle() );
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
Sd3drFaceList Sd3drModel::faceListWall(const Sd3drFace &face1, const Sd3drFace &face2, bool close)
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




Sd3drFaceList Sd3drModel::faceListWallList(const Sd3drFaceList &faceList1, const Sd3drFaceList &faceList2, bool close)
  {
  Sd3drFaceList walls;
  if( faceList1.count() != faceList2.count() )
    return walls;

  for( int i = 0; i < faceList1.count(); i++ )
    walls.append( faceListWall( faceList1.at(i), faceList2.at(i), close ) );

  return walls;
  }




Sd3drFaceList Sd3drModel::faceListWallIndexed(const Sd3drFace &face1, const Sd3drFace &face2, const QList<float> &indexes)
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




Sd3drFaceList Sd3drModel::faceListIndexed(const Sd3drFaceList &faceList, const QList<float> &indexes)
  {
  Sd3drFaceList walls;

  for( auto v : indexes ) {
    int i = static_cast<int>(v);
    if( i < 0 ){
      //Backward counting
      i = faceList.count() + i;
      }
    if( i < faceList.count() && i >= 0 )
      walls.append( faceList.at(i) );
    }

  return walls;
  }




QList<float> Sd3drModel::afloatArc(float radius, float angleStart, float angleStop, int sideCount )
  {
  QList<float> list;
  if( sideCount < 2 ) sideCount = 2;
  float angleStep = (angleStop - angleStart) / sideCount;
  //Convert degree to radians
  float angle = angleStart * M_PI / 180.0;
  float startx = sin(angle) * radius;
  float starty = cos(angle) * radius;
  for( int i = 0; i < sideCount; i++ ) {
    angleStart += angleStep;
    angle = angleStart * M_PI / 180.0;
    float stopx = sin(angle) * radius;
    float stopy = cos(angle) * radius;
    list.append( stopx - startx );
    list.append( stopy - starty );
    startx = stopx;
    starty = stopy;
    }
  return list;
  }







//!
//! \brief faceListExtrude Extrudes model from source face in direction specified by map
//! \param face            Source face of model
//! \param map             Direction of extruding
//! \return                Solid model: floor, roof and walls
//!
Sd3drFaceList Sd3drModel::faceListExtrude(const Sd3drFace &face, const QMatrix4x4 &map)
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
Sd3drFaceList Sd3drModel::faceListExtrudeShift(const Sd3drFace &face, float shift)
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
Sd3drFaceList Sd3drModel::faceListBodyBeveled(float bodyLenght, float bodyWidth, float bodyHeight, float bevelFront, float bevelSide, float verticalHeight )
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
//! \param map          Mapping matrix for box
//! \return             Box model
//!
Sd3drFaceList Sd3drModel::faceListBox(float lenght, float width, float height, const QMatrix4x4 &map )
  {
  return faceListExtrudeShift( faceRectangle( lenght, width, map ), height );
  }



//!
//! \brief faceListCylinder  Builds cylinder model from its size
//! \param radius            Radius of circle of footing of cylinder
//! \param height            Height of cylinder
//! \return                  Cylinder model
//!
Sd3drFaceList Sd3drModel::faceListCylinder(float radius, float height, const QMatrix4x4 &map)
  {
  return faceListExtrudeShift( faceCircleSide( radius, qMax( 20, static_cast<int>(radius * 6.28) ), map ), height );
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
Sd3drFaceList Sd3drModel::faceListHexagon(float lenght, float topLenght, float height, float width)
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




Sd3drFaceList Sd3drModel::faceListCurveXZ(const Sd3drFace &face, float radius, float angleSrc, float angleDst, int sideCount, bool close )
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




Sd3drFaceList Sd3drModel::faceListPinCurveOne(const Sd3drFace &face, float firstLen, float middleLen, float radius, float angle, int sideCount)
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




Sd3drFaceList Sd3drModel::faceListPinCurveTwo(const Sd3drFace &face, float firstLen, float middleLen, float lastLen, float radius, float angleFirst, float angleSecond, int sideCount)
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




Sd3drFaceList Sd3drModel::faceListPinTqfp(float width, float thickness, float fullLenght, float plateLenght, float height)
  {
  return faceListPinCurveTwo( faceRectangle( width, thickness, QMatrix4x4()), fullLenght - plateLenght, height, plateLenght, thickness, 90, -90, 30 );
  }




Sd3drFaceList Sd3drModel::faceListWallRound(const Sd3drFace &face1, const Sd3drFace &face2, float scaleX, float scaleY, float radius, float stepDegree)
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




Sd3drFaceList Sd3drModel::faceListWallBevelXY(const Sd3drFace &face1, const Sd3drFace &face2, float radius, float stepDegree)
  {
  float sizex,sizey;
  faceSizeXY( face1, sizex, sizey );
  return faceListWallRound( face1, face2, (sizex + radius) / sizex, (sizey + radius) / sizey, radius, stepDegree );
  }





Sd3drFaceList Sd3drModel::faceListWallDoubleBevelXY(const Sd3drFace &face1, const Sd3drFace &face2, float radius1, float radius2, float stepDegree1, float stepDegree2, float height)
  {
  Sd3drFaceList walls;
  float midHeight = height - (fabs(radius1) + fabs(radius2));
  Sd3drFace mid0 = faceBevelXY( face1, radius1 );
  Sd3drFace mid1 = midHeight > 0 ? faceDuplicateShift( mid0, midHeight ) : mid0;
  float sizex,sizey;
  faceSizeXY( face1, sizex, sizey );
  walls.append( faceListWallRound( face1, mid0, (sizex + radius1) / sizex, (sizey + radius1) / sizey, fabs(radius1), stepDegree1 ) );
  if( midHeight > 0 )
    walls.append( faceListWall( mid0, mid1, true ) );
  faceSizeXY( mid1, sizex, sizey );
  walls.append( faceListWallRound( mid1, face2, (sizex + radius2) / sizex, (sizey + radius2) / sizey, fabs(radius2), stepDegree2 ) );
  return walls;
  }



Sd3drFaceList Sd3drModel::faceListRotation(const QList<float> &pairList, float angleStart, float angleStop, int sideCount, QMatrix4x4 transfer )
  {
  Sd3drFaceList faceList;

  //Build source face
  int count = pairList.count() / 2;
  if( count < 2 )
    return faceList;
  count <<= 1;
  QVector3D v;
  QList<QVector3D> pathSrc;
  for( int i = 0; i < count; i += 2 ) {
    QVector3D dv( pairList.at(i), 0, pairList.at(i+1) );
    v += dv;
    pathSrc.append( v );
    }

  //Build rotation wires
  QList< QList<QVector3D> > wires;
  if( sideCount < 2 ) sideCount = 2;
  float angleStep = (angleStop - angleStart) / static_cast<float>(sideCount);
  for( int i = 0; i <= sideCount; i++ ) {
    //Rotation matrix
    QMatrix4x4 mat;
    mat.rotate( angleStart, 0, 0, 1.0 );

    //Build next path
    QList<QVector3D> pathDst;
    pathDst.reserve( pathSrc.count() );
    for( auto v : pathSrc )
      pathDst.append( mat.map( v ) );

    //Append to dest wires
    wires.append( pathDst );

    //Next angle
    angleStart += angleStep;
    }

  //Convert wires to layer faces
  for( int layerIndex = 0; layerIndex < pathSrc.count(); layerIndex++ ) {
    Sd3drFace face;
    face.reserve( sideCount + 1 );
    for( int vertexIndex = 0; vertexIndex <= sideCount; vertexIndex++ ) {
      face.append( vertexAppend( transfer.map(wires.at(vertexIndex).at(layerIndex))) );
      }
    faceList.append( face );
    }
  return faceList;
  }




//!
//! \brief faceListWalls Builds walls on base layers, count of element each of them is equals each other
//! \param layers        List of faces throught which will be builds walls
//! \param close         If true then append wall with n-1 and 0 index vertex on each layer
//! \return              List of walls
//!
Sd3drFaceList Sd3drModel::faceListWalls(const Sd3drFaceList &layers, bool close)
  {
  Sd3drFaceList walls;
  for( int i = 1; i < layers.count(); i++ )
    walls.append( faceListWall( layers.at(i-1), layers.at(i), close ) );
  return walls;
  }








//!
//! \brief json Overloaded function to write object content into json writer
//!             Overrided function
//! \param js   Json writer
//!
void Sd3drModel::json(SvJsonWriter3d &js) const
  {
  json3dRegion( js, QStringLiteral("Vertex"), mVertexList );
  js.jsonList( js, QStringLiteral("Instance"), mInstanceList );
  }





//!
//! \brief json Overloaded function to read object content from json reader
//!             Overrided function
//! \param js   Json reader
//!
void Sd3drModel::json(const SvJsonReader3d &js)
  {
  json3dRegion( js, QStringLiteral("Vertex"), mVertexList );
  js.jsonList( js, QStringLiteral("Instance"), mInstanceList );
  }




//!
//! \brief draw3d Draws model in 3d space
//! \param f      OpenGL functions
//! \param mat    Conversion matrix for model
//!
void Sd3drModel::draw3d(QOpenGLFunctions_2_0 *f, QMatrix4x4 mat) const
  {
  for( auto const &instance : mInstanceList )
    instance.draw( f, mVertexList, mat );
  }





//!
//! \brief volumeAdd Append volume of model to result volume
//! \param volume    Source and result volume
//!
void Sd3drModel::volumeAdd(QMatrix2x3 &volume) const
  {
  for( auto const &inst : std::as_const(mInstanceList) ) {
    inst.volumeAdd( volume, mVertexList );
    }
  }



void Sd3drModel::faceSizeXY(const Sd3drFace &face, float &sizex, float &sizey) const
  {
  QPointF p = point( face.at(0) );
  float minX = p.x();
  float maxX = p.x();
  float minY = p.y();
  float maxY = p.y();
  for( auto index : face ) {
    p = point( index );
    if( minX > p.x() ) minX = p.x();
    if( maxX < p.x() ) maxX = p.x();
    if( minY > p.y() ) minY = p.y();
    if( maxY < p.y() ) maxY = p.y();
    }

  sizex = maxX - minX;
  sizey = maxY - minY;
  }
