#ifndef SDM3DTYPES_H
#define SDM3DTYPES_H

#include <QString>
#include <QVector3D>
#include <QMatrix4x4>
#include <QList>
#include <QColor>

#define SDM3D_TYPE_FLOAT   'd'
#define SDM3D_TYPE_COLOR   'c'
#define SDM3D_TYPE_STRING  's'
#define SDM3D_TYPE_VERTEX  'v'
#define SDM3D_TYPE_MATRIX  'm'
#define SDM3D_TYPE_SEGMENT 'l'
#define SDM3D_TYPE_PATH    'p'
#define SDM3D_TYPE_REGION  'r'
#define SDM3D_TYPE_FACE    'f'
#define SDM3D_TYPE_MODEL   'b'

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
