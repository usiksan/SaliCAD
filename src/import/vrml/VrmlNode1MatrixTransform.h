#ifndef VRMLNODE1MATRIXTRANSFORM_H
#define VRMLNODE1MATRIXTRANSFORM_H

#include "VrmlNode1.h"

class VrmlNode1MatrixTransform : public VrmlNode1
  {
  public:
    VrmlNode1MatrixTransform();

    // VrmlNode interface
  public:
    virtual bool parse(SdScanerVrml *scaner, const QString &fieldType) override;
  };

#endif // VRMLNODE1MATRIXTRANSFORM_H
