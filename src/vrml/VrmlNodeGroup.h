#ifndef VRMLNODEGROUP_H
#define VRMLNODEGROUP_H

#include "VrmlVector.h"
#include "VrmlNodeCompound.h"

class VrmlNodeGroup : public VrmlNodeCompound
  {
    VrmlVector mBoxCenter;
    VrmlVector mBoxSize;
  public:
    VrmlNodeGroup();

    void cloneNodeGroup(VrmlNodeGroup *destNode) const;
    // VrmlNode interface
  public:
    virtual VrmlNode *copy() const override;
    virtual void      parse(SdScanerVrml *scaner) override;
    bool              parse2GroupComponents(SdScanerVrml *scaner , const QString nodeType);
  };

#endif // VRMLNODEGROUP_H
