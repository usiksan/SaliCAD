#ifndef NURBSBASIS_H
#define NURBSBASIS_H

#include "objects/SdDoubleVector.h"

#include <QVector3D>
#include <QList>

class NurbsBasis
  {
    SdDoubleVector mKnotVector;
    int            mOrder;
    int            mNBasis;
    double         mUBegin = -1;
    double         mUEnd = 1;
  public:

    //variables

    //Constructor
    NurbsBasis() : mOrder(0), mNBasis(0), mUBegin(-1), mUEnd(1) {}
    NurbsBasis( QList<double> U );

    int  order() const { return mOrder; }

    //simple algorithm
    int  findSpan(double u) const;
    void setRange(double ubegin, double uend);
    int  insert(const double u, const int r);
    void dersBasisFuns(const double u, const int i,const int dk, QList< QList<double> > &ders) const;
    void reverse();
    // return shapefun N which starts from i0-th to i1-th shape functions
    void basisFuns( double u, SdDoubleVector &N,int &i0, int &i1) const;

    //export plotting
    void exportBasisScatter( QList< QList<QVector3D> > &scatters );

  private:
    void normlized();

  };

#endif // NURBSBASIS_H
