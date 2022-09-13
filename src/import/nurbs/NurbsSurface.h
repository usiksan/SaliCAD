#ifndef NURBSSURFACE_H
#define NURBSSURFACE_H

#include "NurbsCurve.h"
#include "NurbsSurfaceInfo.h"
#include "NurbsConvexHull.h"
#include "3dr/Sd3drModel.h"

#include <QVector4D>

class NurbsSurface
  {
    QList<NurbsBasis>         mBasis;   // basis0-> xdir, basis1 ->ydir
    QList< QList<QVector3D> > mControlPoints; // nbasis0 rows, nbasis1 column controlPoints[row][column]
    QList< QList<QVector4D> > mControlPointsWithWeight;			// controlPoints with weight (4D)
    QList<SdDoubleVector>     mWeightVector;  // nbasis0 rows, nbasis1 column
    QList< QList<QVector3D> > mReferPoints;// used to determine the initial guess points
    bool                      mIsPlane = false;
    QList<double>             mPlaneCoe;  // plane : plane_coe[0] * x + plane_coe[1] * y + plane_coe[2] * z + plane_coe[3] = 0
    int                       mNumReferPoints = 11;							// num of refPoints
    double                    mAbsTOL = 1e-7;
    std::string               mName;
    int                       mId;												// id and name are used for any usage that required
    QList<NurbsFace>          mConvexHull;
    bool                      mRational = false;
  public:
    NurbsSurface( const QList<NurbsBasis> &basis, const QList< QList<QVector3D> > &cP );
    NurbsSurface( const QList<NurbsBasis> &basis, const QList< QList<QVector3D> > &cP, const QList<SdDoubleVector> &weight );
    NurbsSurface() {}


    void getPw();
    void getCpFromPw();
    void getCoord(const double knot0, const double knot1, QVector3D &pt) const;
    void calculatePerImage(const QVector3D &pt,double &u, double &v) const;
    void calculateLineIntecs(const QVector3D &ptOnLine, const QVector3D &slope, QList<double> &intecUs, QList<QVector3D> &intecPts) const;
    void projPtInitialGuessByLS(const QVector3D &pt, double &u0, double &u1);
    void _getGridPoints( const int numPoints, NurbsSurfaceInfo &s );


    //Advance alogirthm
    void surfaceKnotIns(const int dir, const double u, const int r);
    void getConvexHull();
    void surfacePointInv(const QVector3D &pt, QVector3D &projPt, double &u, double &v);
    void calDist(const QVector3D &pt, QVector3D &projPt);
    bool lineIntersection(const QVector3D &nearPt, const QVector3D &seg0, const QVector3D &seg1, const double TOL, double &u0, double &u1, QVector3D &intPt);
    bool lineIntersection(const QVector3D &nearPt, const QVector3D &seg0, const QVector3D &seg1, const double TOL, QVector3D &intPt);
    //Export
    void getScattors( const int numPoints, NurbsSurfaceInfo &s);
    void getScattors( NurbsSurfaceInfo &s );
    void offset( double dx, double dy, double dz );

    Sd3drFaceList discrete( int n, Sd3drModel *model ) const;

  private:
    QVector3D     getKnotPoint( double knot0, double knot1 ) const;
    //QList<arma::mat> _mRep;
    int  calCombiation(const int a, const int b) const;
    void calculateMRep();
    void getComp(const int i, const int j, QList<double> &comp) const;
    void surfaceDerivsAlg1(const QList<QList<QVector3D>> &P, const double u,
              const double v, const int d, QList<QList<QVector3D>> &SKL);
    void calMrepVal(const QVector3D &pt, QMatrix4x4 &Mp ) const;
    //void calMrepVal(const QVector3D &pt, arma::mat &Mp) const;
    void ratSurfaceDerives(const int d, const double u, const double v, QList< QList<QVector3D> > &SKL);
  };

#endif // NURBSSURFACE_H
