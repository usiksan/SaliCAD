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

    VrmlColor at( int index ) const { return mColorList.at(index); }

    // VrmlNode interface
  public:
    virtual VrmlNode *copy() const override { return new VrmlNodeColor( this ); }
    virtual bool      parse(SdScanerVrml *scaner, const QString &fieldType) override;
  };

#endif // VRMLNODECOLOR_H