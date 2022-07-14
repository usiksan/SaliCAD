#include "NurbsConvexHull.h"

#include <math.h>

#define AbsTOL 1e-7



NurbsConvexHull::NurbsConvexHull(const QList< QList<QVector3D> > &vertex)
  {
  for( const auto &list : vertex ) {
    for( const auto &p : list ) {
      mArray.append( p );
      }
    }
  int count = mArray.count();
  // find four points that make a tetrahedron whoes volumn is not zero
  QList<int> initList;
  bool tetraFlag = false;
  for( int i = 0; i < count; i++ ) {
    for( int j = i + 1; j < count; j++ ) {
      for( int k = j + 1; k < count; k++ ) {
        // these three points mush not be collinear
        double x0 = mArray.at(i).x(), y0 = mArray.at(i).y(), z0 = mArray.at(i).z();
        double x1 = mArray.at(j).x(), y1 = mArray.at(j).y(), z1 = mArray.at(j).z();
        double x2 = mArray.at(k).x(), y2 = mArray.at(k).y(), z2 = mArray.at(k).z();
        double n[3];
        n[0] = (y1 - y0) * (z2 - z1) - (z1 - z0) * (y2 - y1);
        n[1] = -(x1 - x0) * (z2 - z1) + (z1 - z0) * (x2 - x1);
        n[2] = (x1 - x0) * (y2 - y1) - (y1 - y0) * (x2 - x1);
        double norm = 0.0;
        for( double d : n )
          norm += d * d;
        norm = sqrt(norm);
        if( abs(norm) < AbsTOL )
          continue;

        for( double &d : n )
          d /= norm;

        for( int ii = k + 1; ii < count; ii++ ) {
          //fourth point should not on the plane from plane(i,j,k)
          double D = 0.0;
          double distance = 0.0;
          for( int jj = 0; jj < 3; jj++ ) {
            distance += n[jj] * mArray.at(ii)[jj];
            D -= mArray.at(i)[jj] * n[jj];
            }
          if( abs(distance + D) > AbsTOL ) {
            tetraFlag = true;
            initList.append(i);
            initList.append(j);
            initList.append(k);
            initList.append(ii);
            break;
            }
          }
        if( tetraFlag ) break;
        }
      if( tetraFlag ) break;
      }
    if( tetraFlag ) break;
    }
  if( initList.empty() )
    return;

  /* Initially construct the hull as containing only the first four points. */
  NurbsFace f;
  memset( mE, -1, sizeof(mE) );

  for( int i = 0; i < 4; i++ )
    for( int j = i + 1; j < 4; j++ )
      for( int k = j + 1; k < 4; k++ ) {
        //int rest_idx = initList[6 - i - j - k];
        mFaces.append( makeFace(initList[i], initList[j], initList[k], initList[6 - i - j - k]) );
        }

  /* Now add a point into the hull one at a time. */
  for( int i = 0; i < count; i++ ) {
    bool multiFlag = false;
    for( int ii = 0; ii < 4; ii++ ) {		// same in index
      if( initList[ii] == i ) {
        multiFlag = true;
        break;
        }
      for (int ii = 0; ii < 4; ii++) {		//remove duplicated pts
        QVector3D tmp = mArray.at(i) - mArray.at(initList[ii]);
        double dist2 = QVector3D::dotProduct( tmp, tmp );
        if( dist2 < AbsTOL ) {
          multiFlag = true;
          break;
          }
        }
      if( multiFlag )
        break;
      }

    for( int ii = 0; ii < i; ii++ ) {		// same in coordinate
      QVector3D tmp = mArray.at(i) - mArray.at(ii);
      double dist2 = QVector3D::dotProduct( tmp, tmp );
      if( dist2 < AbsTOL ) {
        multiFlag = true;
        break;
        }
      }
    if( multiFlag )
      continue;

    /* Find and delete all faces with their outside 'illuminated' by this point. */
    for( int j = 0; j < mFaces.size(); j++ ) {
      f = mFaces.at(j);
      double dist = QVector3D::dotProduct( f.mNormal, mArray.at(i) );
      if( dist - f.mDisc > AbsTOL ) { // not illuminated face
        mE[ f.mVertexIndexes[0] ][ f.mVertexIndexes[1] ].erase( f.mVertexIndexes[2] );
        mE[ f.mVertexIndexes[0] ][ f.mVertexIndexes[2] ].erase( f.mVertexIndexes[1] );
        mE[ f.mVertexIndexes[1] ][ f.mVertexIndexes[2] ].erase( f.mVertexIndexes[0] );
        mFaces[j--] = mFaces.takeLast();
        }
      }

    /* Now for any edge still in the hull that is only part of one face
       add another face containing the new point and that edge to the hull. */
    int nfaces = mFaces.size();
    for( int j = 0; j < nfaces; j++ ) {
      f = mFaces.at(j);
      for( int a = 0; a < 3; a++ )
        for( int b = a + 1; b < 3; b++ ) {
          int c = 3 - a - b;
          if( mE[ f.mVertexIndexes[a] ][ f.mVertexIndexes[b] ].size() == 2 )
            continue;
          if( abs( QVector3D::dotProduct( f.mNormal, mArray.at( f.mVertexIndexes[c] ) ) - f.mDisc ) < AbsTOL) { // f.I[c] must not be on the face of f.I[a], f.I[b] , i
            bool findFlag = false;
            for( int iFace = 0; iFace < mFaces.size(); iFace++ ) {
              for( int iPt = 0; iPt < 3; iPt++ )
                if( abs( QVector3D::dotProduct( f.mNormal, mArray.at( mFaces.at(iFace).mVertexIndexes[iPt]) ) - f.mDisc) > AbsTOL) {
                  mFaces.append( makeFace( f.mVertexIndexes[a], f.mVertexIndexes[b], i, mFaces.at(iFace).mVertexIndexes[iPt] )  );
                  findFlag = true;
                  break;
                  }
              if( findFlag )
                break;
              }
            }
          else
            mFaces.append( makeFace( f.mVertexIndexes[a], f.mVertexIndexes[b], i, f.mVertexIndexes[c] )  );
          }
      }
    }
  }



NurbsFace NurbsConvexHull::makeFace(int i, int j, int k, int inside_i)
  {
  mE[i][j].insert(k);
  mE[i][k].insert(j);
  mE[j][k].insert(i);

  NurbsFace f;
  f.mVertexIndexes[0] = i;
  f.mVertexIndexes[1] = j;
  f.mVertexIndexes[2] = k;
  f.mNormal = (mArray.at(j) - mArray.at(i)) * (mArray.at(k) - mArray.at(i));
  f.mDisc = QVector3D::dotProduct( f.mNormal, mArray.at(i) ); //-D
  if( QVector3D::dotProduct( f.mNormal, mArray.at(inside_i) ) > f.mDisc ) {
    f.mNormal = -f.mNormal;
    f.mDisc   = -f.mDisc;
    }
  return f;
  }
