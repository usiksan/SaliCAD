#include "IgesEntityRationalBSplineSurface.h"
#include "IgesReader.h"

#include <QDebug>


IgesEntityRationalBSplineSurface::IgesEntityRationalBSplineSurface()
  {
  qDebug() << "Entity rational BSpline surface";
  }


bool IgesEntityRationalBSplineSurface::parse(IgesReader *reader)
  {
  if( !IgesEntity::parse(reader) ) return false;

  if( !reader->paramInt(mK1) ) return false;
  if( !reader->paramInt(mK2) ) return false;
  if( !reader->paramInt(mM1) ) return false;
  if( !reader->paramInt(mM2) ) return false;
  if( !reader->paramInt(mClosedFirst) ) return false;
  if( !reader->paramInt(mClosedSecond) ) return false;
  if( !reader->paramInt(mPolynominal) ) return false;
  if( !reader->paramInt(mPeriodicFirst) ) return false;
  if( !reader->paramInt(mPeriodicSecond) ) return false;

  int n1 = 1 + mK1 - mM1;
  int n2 = 1 + mK2 - mM2;
  int a = n1 + 2 * mM1;
  int b = n2 + 2 * mM2;
  int c = (1 + mK1) * (1 + mK2);

  if( !paramDoubleVector( reader, mFirstKnotSequence, a+1 ) ) return false;
  if( !paramDoubleVector( reader, mSecondKnotSequence, b+1 ) ) return false;
  if( !paramDoubleVector( reader, mWeight, c ) ) return false;
  if( !paramDoubleVector( reader, mControlPoints, 3 * c ) ) return false;

  if( !reader->paramReal(mU0) ) return false;
  if( !reader->paramReal(mU1) ) return false;
  if( !reader->paramReal(mV0) ) return false;
  if( !reader->paramReal(mV1) ) return false;
  return true;
  }
