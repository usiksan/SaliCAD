#ifndef VRMLNODE1_H
#define VRMLNODE1_H

#include "VrmlNode.h"

class VrmlNode1 : public VrmlNode
  {
  public:
    VrmlNode1() : VrmlNode() {}

    static VrmlNode  *parse1Node( SdScanerVrml *scaner, const QString nodeType );
    static VrmlNode  *build1Node( const QString nodeType );

    // VrmlNode interface
  public:
    virtual VrmlNode *copy() const override { return nullptr; }
  };

#endif // VRMLNODE1_H
