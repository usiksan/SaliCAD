#ifndef SDM3DTYPES_H
#define SDM3DTYPES_H

#include <QString>
#include <QVector3D>
#include <QMatrix4x4>
#include <QList>
#include <QColor>

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

using SdM3dFaceSet = QList<SdM3dFace>;

#endif // SDM3DTYPES_H
