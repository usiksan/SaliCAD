#ifndef VRMLNODECOLOR_H
#define VRMLNODECOLOR_H

#include "VrmlColor.h"
#include "VrmlNode.h"

class VrmlNodeColor : public VrmlNode
  {
    VrmlColorList mColorList;
  public:
    VrmlNodeColor();
    VrmlNodeColor( const VrmlNodeColor *color ) : VrmlNode(color), mColorList(color->mColorList) {}

    // VrmlNode interface
  public:
    virtual void      parse(SdScanerVrml *scaner) override;
    virtual VrmlNode *copy() const override { return new VrmlNodeColor( this ); }
  };

#endif // VRMLNODECOLOR_H
