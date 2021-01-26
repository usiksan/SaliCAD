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
    VrmlNodeGroup( const VrmlNodeGroup *group );

    // VrmlNode interface
  public:
    virtual VrmlNode *copy() const override { return new VrmlNodeGroup( this ); }
    virtual void      parse(SdScanerVrml *scaner) override;
    bool              parse2GroupComponents(SdScanerVrml *scaner , const QString nodeType);
  };

#endif // VRMLNODEGROUP_H
