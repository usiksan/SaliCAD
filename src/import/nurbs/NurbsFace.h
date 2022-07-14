#ifndef NURBSFACE_H
#define NURBSFACE_H

#include <QVector3D>

struct NurbsFace
  {
    QVector3D mNormal;
    double    mDisc;
    int       mVertexIndexes[3];
  };

#endif // NURBSFACE_H
