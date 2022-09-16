#ifndef VRMLNODECOLOR_H
#define VRMLNODECOLOR_H

#include "VrmlColor.h"
#include "VrmlNode.h"
#include "3dr/Sd3drMaterial.h"

class VrmlNodeColor : public VrmlNode
  {
    VrmlColorList mColorList;
  public:
    VrmlNodeColor();
    VrmlNodeColor( const VrmlNodeColor *color ) : VrmlNode(color), mColorList(color->mColorList) {}

    VrmlColor     at( int index ) const { return mColorList.at(index); }

    Sd3drMaterial material( int index ) const;

    // VrmlNode interface
  public:
    virtual VrmlNode *copy() const override { return new VrmlNodeColor( this ); }
    virtual bool      parse(SdScanerVrml *scaner, const QString &fieldType) override;
  };

#endif // VRMLNODECOLOR_H
