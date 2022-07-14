#ifndef NURBSCURVE_H
#define NURBSCURVE_H

#include "NurbsBasis.h"
#include "NurbsCurveInfo.h"


#include <QVector3D>

class NurbsCurve
  {
    NurbsBasis       mBasis;
    QList<double>    mWeightVector;
    QList<QVector3D> mControlPoints;
    QString          mName;
    int              mId;
    bool             mRational = false;
    double           mAbsTOL = 1e-6;

  protected:
    //std::vector<arma::mat> _mRep;
    void curveDerivsAlg1( const QList<QVector3D> &P, const double u, const int d, const QList<QVector3D> &SKL) const;
    int calCombiation(const int a, const int b) const;
    void getComp(const int i, QList<double> &comp) const;
    void calculateMRep(const int v);
    //void calMrepVal(const QVector3D &pt, arma::mat &Mp) const;
    //void calMrepVal(const NS::Vector3D &pt, arma::mat &Mp) const;
  public:

    void getPw( const QList<QVector3D> &cp, QList<QVector4D> &pw) const;
    void getCpFromPw(const QList<QVector4D> &pw, QList<QVector3D> &cp, std::vector<double> &w) const;
    int  getOrder() const { return mBasis.order(); }
    //void getMRep(std::vector<arma::mat> &mRep) const { mRep = _mRep; };

    void getCoord( const double kont, QVector3D &p ) const;

    void ratCurveDerives(const int d, const double u, QList<QVector3D> &SKL) const;
    void _getScatters(const int numPoints, NurbsCurveInfo &c);
    double arc_length( const double k0, const double k1) const;

    NurbsCurve() { }
    NurbsCurve( const NurbsBasis &b, const QList<QVector3D> &_cP);
    NurbsCurve( const NurbsBasis &b, const QList<QVector3D> &_cP, QList<double> &w );
    NurbsCurve(const NurbsCurve &c);

    void reverse();
    //advance algorithm
    void curveKnotIns(double u, int r);
    double calculatePerImage(const QVector3D &pt) const;
    void calculateLineIntecs(const QVector3D &ptOnLine, const QVector3D &slope, const QList<double> &intecUs, const QList<QVector3D> &intecPts) const;

    //io
    void getScattors(const int numPoints, NurbsCurveInfo &c);
    void getScattors(NurbsCurveInfo &c);
  };

#endif // NURBSCURVE_H
