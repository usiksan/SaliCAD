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
    virtual VrmlNode *copy() const override { return new VrmlNodeIndexedFaceSet( this ); }
    virtual bool      parse(SdScanerVrml *scaner, const QString &fieldType) override;
    virtual void      generateFaces(std::function<void (const QVector3DList &, const QVector3DList &, const VrmlNodeMaterial *)> appendFace) const override;
  };

#endif // VRMLNODEINDEXEDFACESET_H
