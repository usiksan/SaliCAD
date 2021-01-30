#ifndef VRMLNODE1MATERIAL_H
#define VRMLNODE1MATERIAL_H

#include "VrmlNode1.h"

class VrmlNode1Material : public VrmlNode1
  {
  public:
    VrmlNode1Material();

    // VrmlNode interface
  public:
    virtual bool parse(SdScanerVrml *scaner, const QString &fieldType) override;
  };

#endif // VRMLNODE1MATERIAL_H
