#include "IgesEntityTrimmedParametricSurface.h"
#include "IgesReader.h"

IgesEntityTrimmedParametricSurface::IgesEntityTrimmedParametricSurface() :
  IgesEntityFaceGenerator(),
  mTrimmedSurface(nullptr),
  mDiffersBoundary(0),
  mOuterBoundary(nullptr)
  {

  }


bool IgesEntityTrimmedParametricSurface::parse(IgesReader *reader)
  {
  // The associated parameter data are:
  // + PTS: Pointer: surface to be trimmed
  // + N1: Int: 0 = self-bounded PTS, 1 = boundary of PTS differs from PTO
  // + N2: Number of internal boundaries (cutouts)
  // + PTO: Pointer: outer boundary of the surface
  // + PTI: Pointer: list of inner boundaries (cutouts)

  if( !IgesEntity::parse(reader) ) return false;

  if( !reader->paramEntityExPtr( mTrimmedSurface ) ) return false;
  if( !reader->paramInt( mDiffersBoundary ) ) return false;
  int innerBoundCount;
  if( !reader->paramInt( innerBoundCount ) ) return false;
  if( !reader->paramEntityExPtr( mOuterBoundary ) ) return false;

  for( int i = 0; i < innerBoundCount; i++ ) {
    IgesCurvePtr curve;
    if( !reader->paramEntityExPtr( curve ) ) return false;
    mInnerBoundary.append( curve );
    }

  return true;
  }

void IgesEntityTrimmedParametricSurface::generateFaces(Sd3drModel *model)
  {
  }
