#ifndef SD3DRTRANSFER_H
#define SD3DRTRANSFER_H

#include "Sd3dPoint.h"
#include "SdJsonIO.h"

#include <QMatrix4x4>

class Sd3drTransfer
  {
    Sd3dPoint mOffset;
    Sd3dPoint mRotate;
    double    mAngle;
  public:
    Sd3drTransfer();

    QMatrix4x4 mapGet() const;
  };

#endif // SD3DRTRANSFER_H
