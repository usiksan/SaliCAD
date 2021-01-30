#ifndef VRMLNODE1ROTATION_H
#define VRMLNODE1ROTATION_H

#include "VrmlNode1.h"

class VrmlNode1Rotation : public VrmlNode1
  {
  public:
    VrmlNode1Rotation();

    // VrmlNode interface
  public:
    virtual bool parse(SdScanerVrml *scaner, const QString &fieldType) override;
  };

#endif // VRMLNODE1ROTATION_H
