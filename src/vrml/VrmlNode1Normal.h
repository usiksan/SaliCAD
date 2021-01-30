#ifndef VRMLNODE1NORMAL_H
#define VRMLNODE1NORMAL_H

#include "VrmlNode1.h"

class VrmlNode1Normal : public VrmlNode1
  {
  public:
    VrmlNode1Normal();

    // VrmlNode interface
  public:
    virtual bool parse(SdScanerVrml *scaner, const QString &fieldType) override;
  };

#endif // VRMLNODE1NORMAL_H
