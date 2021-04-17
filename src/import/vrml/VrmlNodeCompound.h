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
    virtual void generateFaces(std::function<void (const QVector3DList &, const QVector3DList &, const VrmlNodeMaterial *)> appendFace) const override;
  };

#endif // VRMLNODECOMPOUND_H
