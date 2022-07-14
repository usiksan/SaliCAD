#ifndef NURBSPARENTSURFACE_H
#define NURBSPARENTSURFACE_H

#include "NurbsSurface.h"

class NurbsParentSurface : public NurbsSurface
  {
    QList<NurbsSurface> mChildren;
  public:
    NurbsParentSurface() {}
    NurbsParentSurface( const QList<NurbsBasis> &basis, const QList< QList<QVector3D> > &cP);
    NurbsParentSurface( const QList<NurbsBasis> &basis, const QList< QList<QVector3D> > &cP, const QList< QList<double> > &weight );

    void subDivision();
    void offset(double dx, double dy, double dz);
    int  containsPoint( const QVector3D &pt, QList<int> &idx, double TOL_ratio ) const;
  };

#endif // NURBSPARENTSURFACE_H
