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
    virtual VrmlNode *copy() const override { return new VrmlNodeColor( this ); }
    virtual bool      parse(SdScanerVrml *scaner, const QString &fieldType) override;
    virtual quint32   colorGet(int index, quint32 color) override;
  };

#endif // VRMLNODECOLOR_H
