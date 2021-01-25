#ifndef VRMLNODEINDEXEDFACESET_H
#define VRMLNODEINDEXEDFACESET_H

#include "VrmlNode.h"
#include "VrmlNodeColor.h"
#include "VrmlNodeCoordinate.h"
#include "VrmlNodeNormal.h"

class VrmlNodeIndexedFaceSet : public VrmlNode
  {
    VrmlNodeColor      *mColor;
    VrmlNodeCoordinate *mCoordinate;
    VrmlNodeNormal     *mNormal;
    VrmlBool            mCcw;
    VrmlInt32List       mColorIndex;
    VrmlBool            mColorPerVertex;
    VrmlBool            mConvex;
    VrmlInt32List       mCoordIndex;
    VrmlFloat           mCreaseAngle;
    VrmlInt32List       mNormalIndex;
    VrmlBool            mNormalPerVertex;
    VrmlBool            mSolid;
  public:
    VrmlNodeIndexedFaceSet();
  };

#endif // VRMLNODEINDEXEDFACESET_H
