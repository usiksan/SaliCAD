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

    static VrmlNodeGroup *parse2Group( SdScanerVrml *scaner );
    static bool           parse2GroupComponents( SdScanerVrml *scaner, VrmlNodeGroup *group );

    void cloneNodeGroup(VrmlNodeGroup *destNode) const;
    // VrmlNode interface
  public:
    virtual VrmlNode *copy() const override;
  };

#endif // VRMLNODEGROUP_H
