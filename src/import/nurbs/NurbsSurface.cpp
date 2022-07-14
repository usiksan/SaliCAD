#include "NurbsSurface.h"


Sd3drFaceList NurbsSurface::discrete(int n, Sd3dModel *model) const
  {
  Sd3drFaceList faceList;

  // Init Knot Vector
  QList<double> k;
  k.reserve(n);
  for( int i = 0; i < n; i++ )
    k.append( -1.0 + i * 2.0 / (n - 1) );

  int startIndex = model->vertexCount();
  // Calculate points for each pair of knot value
  for( int i = 0; i < n; i++ )
    for( int j = 0; j < n; j++ )
      model->vertexAppend( getKnotPoint( k.at(i), k.at(j) )   );

  //Build faces
  for (int i = 0; i < n - 1; i++)
    for (int j = 0; j < n - 1; j++) {
      faceList.append( { startIndex + i*n + j, startIndex + i*n + j + 1, startIndex + (i + 1)*n + j } );
      faceList.append( { startIndex + i*n + j + 1, startIndex + (i + 1)*n + j + 1, startIndex + (i + 1)*n + j } );
      }

  return faceList;
  }




QVector3D NurbsSurface::getKnotPoint(double knot0, double knot1) const
  {
  QVector3D pt(0.0, 0.0, 0.0);
  double knot[] = { knot0,knot1 };
  SdDoubleVector N[2];
  int i0[2];
  int i1[2];
  for( int i = 0; i < 2; i++ )
    mBasis.at(i).basisFuns(knot[i], N[i], i0[i], i1[i]);


  double denu = 0.0; // only for rational
  for( int i = i0[0]; i <= i1[0]; i++ ) {
    for( int j = i0[1]; j <= i1[1]; j++ ) {
      if( mRational ) {
        pt += mControlPoints[i][j] * (N[0][i - i0[0]] * N[1][j - i0[1]] * mWeightVector[i][j]);
        denu += N[0][i - i0[0]] * N[1][j - i0[1]] * mWeightVector[i][j];
        }
      else
        pt += mControlPoints[i][j] * (N[0][i - i0[0]] * N[1][j - i0[1]]);
      }
    }
  if( mRational )
    pt /= denu;

  return pt;
  }


