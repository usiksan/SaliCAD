#ifndef VRMLNODE1INDEXEDFACESET_H
#define VRMLNODE1INDEXEDFACESET_H

#include "VrmlNode1.h"

class VrmlNode1IndexedFaceSet : public VrmlNode1
  {
  public:
    VrmlNode1IndexedFaceSet();

    // VrmlNode interface
  public:
    virtual bool parse(SdScanerVrml *scaner, const QString &fieldType) override;
  };

#endif // VRMLNODE1INDEXEDFACESET_H
