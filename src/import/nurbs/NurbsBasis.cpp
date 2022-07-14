#include "NurbsBasis.h"

#include <QVector>
#include <math.h>

#define AbsTOL 1e-6

// calculate the span for later calculating the basis function.
int NurbsBasis::findSpan(double u) const
  {
  //n	Num of Basis - 1
  //p	Order of B-Spline Basis
  //u	The Knot
  //U	Knot Vector
  int low, high, mid;
  if (abs(u - mKnotVector[mNBasis + 1]) < AbsTOL )
    return mNBasis - 1;
  low = mOrder;
  high = mNBasis + 1;
  mid = (low + high) / 2;
  while( u < mKnotVector[mid] || u >= mKnotVector[mid + 1] ) {
    if( u < mKnotVector[mid] )
      high = mid;
    else
      low = mid;
    mid = (low + high) / 2;
    }
  return(mid);
  }




void NurbsBasis::basisFuns(double u, SdDoubleVector &N, int &i0, int &i1) const
  {
  int iSpan = findSpan(u);

  assert(u >= -1 && u <= 1);
  N.resize(mOrder + 1);
  N[0] = 1.0;
  SdDoubleVector left(mOrder + 1), right(mOrder + 1);
  for( int j = 1; j <= mOrder; j++ ) {
    left[j] = u - mKnotVector[iSpan + 1 - j];
    right[j] = mKnotVector[iSpan + j] - u;
    double saved = 0.0;
    for (int r = 0; r < j; r++) {
      double temp = N[r] / (right[r + 1] + left[j - r]);
      N[r] = saved + right[r + 1] * temp;
      saved = left[j - r] * temp;
      }
    N[j] = saved;
    }

  i0 = iSpan - mOrder;
  i1 = iSpan;
  }
