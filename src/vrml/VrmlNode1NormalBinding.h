#ifndef VRMLNODE1NORMALBINDING_H
#define VRMLNODE1NORMALBINDING_H

#include "VrmlNode1.h"

class VrmlNode1NormalBinding : public VrmlNode1
  {
  public:
    VrmlNode1NormalBinding();

    // VrmlNode interface
  public:
    virtual bool parse(SdScanerVrml *scaner, const QString &fieldType) override;
  };

#endif // VRMLNODE1NORMALBINDING_H
