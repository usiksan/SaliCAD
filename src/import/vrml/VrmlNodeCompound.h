#ifndef VRMLNODECOMPOUND_H
#define VRMLNODECOMPOUND_H

#include "VrmlNode.h"

class VrmlNodeCompound : public VrmlNode
  {
  protected:
    VrmlNodePtrList mChildren;
  public:
    VrmlNodeCompound();
    VrmlNodeCompound( const VrmlNodeCompound *compound );
    virtual ~VrmlNodeCompound();

    void parseChildren(SdScanerVrml *scaner);

    // VrmlNode interface
  public:
    virtual void generateFaces( Sd3drModel *model, Sd3drInstance &instance, Sd3drBody &body ) const override;
  };

#endif // VRMLNODECOMPOUND_H
