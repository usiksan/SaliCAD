#ifndef SDM3DTYPES_H
#define SDM3DTYPES_H

#include <QString>
#include <QVector3D>
#include <QMatrix4x4>
#include <QList>
#include <QColor>

#define SDM3D_TYPE_NONE    '_'
//basis
#define SDM3D_TYPE_BOOL    'b'
#define SDM3D_TYPE_FLOAT   'd'
#define SDM3D_TYPE_COLOR   'c'
#define SDM3D_TYPE_STRING  's'
//2d
#define SDM3D_TYPE_FLAT    't'
//3d
#define SDM3D_TYPE_VERTEX  'v'
#define SDM3D_TYPE_MATRIX  'x'
#define SDM3D_TYPE_SEGMENT 'l'
#define SDM3D_TYPE_PATH    'p'
#define SDM3D_TYPE_REGION  'r'
#define SDM3D_TYPE_FACE    'f'
#define SDM3D_TYPE_MODEL   'm'


class SdM3dFlat {
    QPoint  mPos;
    QPoint  mPosB;
    QPoint  mPosName;
    QString mPad;
    QString mPinNumber;
    QString mPinNumberAttr;
    QString mPinNameAttr;
  public:
    enum {
      sdm2dLine,
      sdm2dRect,
      sdm2dCircle,
      sdm2dFRect,
      sdm2dPin,
      sdm2dIdent,
      sdm2dValue
    }       mType;

    QPoint  pointA() const { return mPos; }
    QPoint  pointB() const { return mPosB; }

    QPoint  circleCenter() const { return mPos; }
    int     circleRadius() const { return mPosB.x(); }

    QPoint  pinPos() const { return mPos; }
    QString pinPad() const { return mPad; }
    QPoint  pinNumberPos() const { return mPosB; }
    QString pinNumber() const { return mPinNumber; }
    QString pinNumberAttr() const { return mPinNumberAttr; }
    QPoint  pinNamePos() const { return mPosName; }
    QString pinNameAttr() const { return mPinNameAttr; }

    QPoint  identPos() const { return mPos; }
    QString identAttr() const { return mPinNameAttr; }

    QPoint  valuePos() const { return mPos; }
    QString valueAttr() const { return mPinNameAttr; }

    QPoint  vectorToPoint( QVector3D p ) const { p *= 1000.0; return p.toPoint(); }

    //Build line
    SdM3dFlat( QVector3D p1, QVector3D p2 ) { mType = sdm2dLine; mPos = vectorToPoint(p1); mPosB = vectorToPoint(p2); }

    //Build rect or frect
    SdM3dFlat( QVector3D p1, QVector3D p2, bool filled ) { mType = filled ? sdm2dFRect : sdm2dRect; mPos = vectorToPoint(p1); mPosB = vectorToPoint(p2); }

    //Build circle
    SdM3dFlat( QVector3D center, float radius ) { mType = sdm2dCircle; mPos = vectorToPoint(center); mPosB.setX( radius * 1000.0 ); }

    //Build pin
    SdM3dFlat( QVector3D pinPos, QString pad, QVector3D numberPos, QString number, QString numberAttr, QVector3D namePos, QString nameAttr ) {
      mType = sdm2dPin;
      mPos = vectorToPoint(pinPos);
      mPad = pad;
      mPosB = vectorToPoint(numberPos);
      mPinNumber = number;
      mPinNumberAttr = numberAttr;
      mPosName = vectorToPoint(namePos);
      mPinNameAttr = nameAttr;
      }

    //Build ident or value place
    SdM3dFlat( QVector3D pos, QString attr, bool isIdent ) {
      mType = isIdent ? sdm2dIdent : sdm2dValue;
      mPos = vectorToPoint(pos);
      mPinNameAttr = attr;
      }
  };





struct SdM3dSegment {
    QVector3D mStart;  //!< Start vertex of segment
    QVector3D mFinish; //!< Finish vertex of segment
  };

using SdM3dPath = QList<SdM3dSegment>;

using SdM3dRegion = QList<QVector3D>;

struct SdM3dFace {
    SdM3dRegion mContour;
    QColor      mColor;
  };

using SdM3dModel = QList<SdM3dFace>;

#endif // SDM3DTYPES_H
