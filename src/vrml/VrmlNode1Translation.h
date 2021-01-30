#ifndef VRMLNODE1TRANSLATION_H
#define VRMLNODE1TRANSLATION_H

#include "VrmlNode1.h"

class VrmlNode1Translation : public VrmlNode1
  {
  public:
    VrmlNode1Translation();

    // VrmlNode interface
  public:
    virtual bool parse(SdScanerVrml *scaner, const QString &fieldType) override;
  };

#endif // VRMLNODE1TRANSLATION_H
