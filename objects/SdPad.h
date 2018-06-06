#ifndef SDPAD_H
#define SDPAD_H

#include "SdPoint.h"

#include <QJsonObject>

class SdPad
  {
    bool   mIsCircle;
    qint32 mWidth;
    qint32 mHeight;
    qint32 mMaskThreshold;
    qint32 mStensilThreshold;
  public:
    SdPad();
  };

#endif // SDPAD_H
