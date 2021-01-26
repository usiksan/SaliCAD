#ifndef VRMLNODEINDEXEDFACESET_H
#define VRMLNODEINDEXEDFACESET_H

#include "VrmlNode.h"
#include "VrmlNodeColor.h"
#include "VrmlNodeCoordinate.h"
#include "VrmlNodeNormal.h"

class VrmlNodeIndexedFaceSet : public VrmlNode
  {
    VrmlNode           *mColor;
    VrmlNode           *mCoordinate;
    VrmlNode           *mNormal;
    VrmlNode           *mTexCoord;
    VrmlBool            mCcw;
    VrmlInt32List       mColorIndex;
    VrmlBool            mColorPerVertex;
    VrmlBool            mConvex;
    VrmlInt32List       mCoordIndex;
    VrmlFloat           mCreaseAngle;
    VrmlInt32List       mNormalIndex;
    VrmlBool            mNormalPerVertex;
    VrmlBool            mSolid;
    VrmlInt32List       mTexCoordIndex;
  public:
    VrmlNodeIndexedFaceSet();
    VrmlNodeIndexedFaceSet( const VrmlNodeIndexedFaceSet *faceSet );
    virtual ~VrmlNodeIndexedFaceSet();

    // VrmlNode interface
  public:
    virtual void      parse(SdScanerVrml *scaner) override;
    virtual VrmlNode *copy() const override { return new VrmlNodeIndexedFaceSet( this ); }
  };

#endif // VRMLNODEINDEXEDFACESET_H
