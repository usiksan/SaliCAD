#ifndef VRMLNODENORMAL_H
#define VRMLNODENORMAL_H

#include "VrmlNode.h"
#include "VrmlVector.h"

class VrmlNodeNormal : public VrmlNode
  {
    VrmlVectorList mVectorList;
  public:
    VrmlNodeNormal() : VrmlNode() {}
    VrmlNodeNormal( const VrmlNodeNormal *normal ) : VrmlNode(normal), mVectorList( normal->mVectorList ) {}

    // VrmlNode interface
  public:
    virtual void      parse(SdScanerVrml *scaner) override;
    virtual VrmlNode *copy() const override { return new VrmlNodeNormal( this ); }
  };

#endif // VRMLNODENORMAL_H
