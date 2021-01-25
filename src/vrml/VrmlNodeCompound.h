#ifndef VRMLNODECOMPOUND_H
#define VRMLNODECOMPOUND_H

#include "VrmlNode.h"

class VrmlNodeCompound : public VrmlNode
  {
  protected:
    VrmlNodePtrList mChildren;
  public:
    VrmlNodeCompound();

    void parseChildren(SdScanerVrml *scaner);

    void cloneNodeCompound( VrmlNodeCompound *destNode ) const;

  };

#endif // VRMLNODECOMPOUND_H
