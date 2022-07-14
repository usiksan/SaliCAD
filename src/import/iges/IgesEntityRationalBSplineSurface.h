#ifndef IGESENTITYRATIONALBSPLINESURFACE_H
#define IGESENTITYRATIONALBSPLINESURFACE_H

#include "IgesEntity.h"

#include <QList>


class IgesEntityRationalBSplineSurface : public IgesEntity
  {
    int mK1; //Upper index of first sum.
    int mK2; //Upper index of second sum.
    int mM1; //Degree of first set of basis functions
    int mM2; //Degree of second set of basis functions
    int mClosedFirst; //1 = Closed in first parametric variable direction;  0 = Not closed
    int mClosedSecond; //1 = Closed in second parametric variable direction;  0 = Not closed
    int mPolynominal;  //0 = Rational; 1 = Polynomial
    int mPeriodicFirst; //0 = Non-periodic in first parametric variable direction; 1 = Periodic in first parametric variable direction
    int mPeriodicSecond; //0 = Non-periodic in second parametric variable direction; 1 = Periodic in second parametric variable direction

    SdDoubleVector mFirstKnotSequence;
    SdDoubleVector mSecondKnotSequence;
    SdDoubleVector mWeight;
    SdDoubleVector mControlPoints;

    double         mU0,mU1;
    double         mV0,mV1;
  public:
    IgesEntityRationalBSplineSurface();

    // IgesEntity interface
  public:
    virtual bool parse(IgesReader *reader) override;

  };

#endif // IGESENTITYRATIONALBSPLINESURFACE_H
