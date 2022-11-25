#ifndef IGESENTITYTRIMMEDPARAMETRICSURFACE_H
#define IGESENTITYTRIMMEDPARAMETRICSURFACE_H

#include "IgesEntityFaceGenerator.h"


// NOTE:
// The associated parameter data are:
// + PTS: Pointer: surface to be trimmed
// + N1: Int: 0 = self-bounded PTS, 1 = boundary of PTS differs from PTO
// + N2: Number of internal boundaries (cutouts)
// + PTO: Pointer: outer boundary of the surface
// + PTI: Pointer: list of inner boundaries (cutouts)
// Forms: 0 only
//
// Unused DE items:
// + Structure
//

class IgesEntitySurface;
class IgesEntityCurveOnParametricSurface;

//!
//! \brief The IgesEntityTrimmedParametricSurface class represents a Trimmed Parametric Surface; this is the
//! primary entity type used to describe surfaces of a solid model.
//!
class IgesEntityTrimmedParametricSurface : public IgesEntityFaceGenerator
  {
    using IgesCurvePtr = IgesEntityCurveOnParametricSurface*;
    // E106-63 (copious data)
    // E108 (plane)
    // E114 (parametric spline surface)
    // E118 (ruled surface)
    // E120 (surface of revolution)
    // E122 (tabulated cylinder)
    // E128 (NURBS surface)
    // E140 (offset surface)
    // E143 (bounded surface)
    // E190 (plane surface)
    // E192 (right circular cylindrical surface)
    // E194 (right circular conical surface)
    // E196 (spherical surface)
    // E198 (toroidal surface)
    IgesEntitySurface   *mTrimmedSurface;
    int                  mDiffersBoundary;
    IgesCurvePtr         mOuterBoundary;
    QList<IgesCurvePtr>  mInnerBoundary;
  public:
    IgesEntityTrimmedParametricSurface();

    // IgesEntity interface
  public:
    virtual bool parse(IgesReader *reader) override;

    // IgesEntityFaceGenerator interface
  public:
    virtual void generateFaces(Sd3drModel *model) override;
  };

#endif // IGESENTITYTRIMMEDPARAMETRICSURFACE_H
