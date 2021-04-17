#ifndef VRMLNODE1TRANSFORM_H
#define VRMLNODE1TRANSFORM_H

#include "VrmlNode1.h"

class VrmlNode1Transform : public VrmlNode1
  {
  public:
    VrmlNode1Transform();

    // VrmlNode interface
  public:
    virtual bool parse(SdScanerVrml *scaner, const QString &fieldType) override;
  };

#endif // VRMLNODE1TRANSFORM_H
