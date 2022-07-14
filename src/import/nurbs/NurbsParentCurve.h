#ifndef NURBSPARENTCURVE_H
#define NURBSPARENTCURVE_H

#include "NurbsCurve.h"

class NurbsParentCurve : public NurbsCurve
  {
    QList<NurbsCurve> mChildren;
  public:
    NurbsParentCurve() {}
    // constructor
    NurbsParentCurve( const NurbsCurve &curve );
    NurbsParentCurve( const NurbsBasis &b, const QList<QVector3D> &_cP );
    NurbsParentCurve( const NurbsBasis &b, const QList<QVector3D> &_cP, const QList<double> &w );

    // getset
    void getChildren( QList<NurbsCurve> &ch ) const { ch = mChildren; }

    //method
    void subDivision();
  };

#endif // NURBSPARENTCURVE_H
