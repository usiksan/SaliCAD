#ifndef NURBSSURFACEINFO_H
#define NURBSSURFACEINFO_H

#include <QVector3D>
#include <QList>

struct NurbsSurfaceInfo
  {
    QList< QList<QVector3D> > mPoints;
    QList< QList<double> >    mKnots;

    NurbsSurfaceInfo( const QList< QList<QVector3D> > &points, const QList< QList<double> > &knots ) : mPoints(points), mKnots(knots) {}
    NurbsSurfaceInfo() {}

  };

#endif // NURBSSURFACEINFO_H
