#ifndef VRMLNODE1MATERIALBINDING_H
#define VRMLNODE1MATERIALBINDING_H

#include "VrmlNode1.h"

class VrmlNode1MaterialBinding : public VrmlNode1
  {
  public:
    VrmlNode1MaterialBinding();

    // VrmlNode interface
  public:
    virtual bool parse(SdScanerVrml *scaner, const QString &fieldType) override;
  };

#endif // VRMLNODE1MATERIALBINDING_H
