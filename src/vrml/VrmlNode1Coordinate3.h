#ifndef VRMLNODE1COORDINATE3_H
#define VRMLNODE1COORDINATE3_H

#include "VrmlNode1.h"

class VrmlNode1Coordinate3 : public VrmlNode1
  {
  public:
    VrmlNode1Coordinate3();

    // VrmlNode interface
  public:
    virtual bool parse(SdScanerVrml *scaner, const QString &fieldType) override;
  };

#endif // VRMLNODE1COORDINATE3_H
