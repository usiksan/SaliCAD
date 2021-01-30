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

    VrmlVector at( int index ) const { return mVectorList.at(index); }

    // VrmlNode interface
  public:
    virtual VrmlNode *copy() const override { return new VrmlNodeNormal( this ); }
    virtual bool      parse(SdScanerVrml *scaner, const QString &fieldType) override;
  };

#endif // VRMLNODENORMAL_H
