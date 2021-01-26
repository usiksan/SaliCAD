#ifndef VRMLNODESHAPE_H
#define VRMLNODESHAPE_H

#include "VrmlNode.h"

class VrmlNodeShape : public VrmlNode
  {
    VrmlNodePtr mApperance;
    VrmlNodePtr mGeometry;
  public:
    VrmlNodeShape();
    VrmlNodeShape( const VrmlNodeShape *shape );
    virtual ~VrmlNodeShape();

    // VrmlNode interface
  public:
    virtual void      parse(SdScanerVrml *scaner) override;
    virtual VrmlNode *copy() const override { return new VrmlNodeShape( this ); }
  };

#endif // VRMLNODESHAPE_H
