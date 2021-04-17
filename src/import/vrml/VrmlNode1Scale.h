#ifndef VRMLNODE1SCALE_H
#define VRMLNODE1SCALE_H

#include "VrmlNode1.h"

class VrmlNode1Scale : public VrmlNode1
  {
  public:
    VrmlNode1Scale();

    // VrmlNode interface
  public:
    virtual bool parse(SdScanerVrml *scaner, const QString &fieldType) override;
  };

#endif // VRMLNODE1SCALE_H
