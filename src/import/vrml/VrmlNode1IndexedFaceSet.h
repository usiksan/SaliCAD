#ifndef VRMLNODE1INDEXEDFACESET_H
#define VRMLNODE1INDEXEDFACESET_H

#include "VrmlNode1.h"

class VrmlNode1IndexedFaceSet : public VrmlNode1
  {
    VrmlInt32List mCoordIndex;
    VrmlInt32List mMaterialIndex;
    VrmlInt32List mNormalIndex;
  public:
    VrmlNode1IndexedFaceSet();


    bool isCoordValid( int index ) const { return index >= 0 && index < mCoordIndex.count(); }

    int  coordIndex( int index ) const { return mCoordIndex.at(index); }


    bool isMaterialValid( int index ) const { return index >= 0 && index < mMaterialIndex.count(); }

    int  materialIndex( int index ) const { return mMaterialIndex.at(index); }


    bool isNormalValid( int index ) const { return index >= 0 && index < mNormalIndex.count(); }

    int  normalIndex( int index ) const { return mNormalIndex.at(index); }


    const VrmlInt32List &coordIndex() const { return mCoordIndex; }

    const VrmlInt32List &materialIndex() const { return mMaterialIndex; }

    const VrmlInt32List &normalIndex() const { return mNormalIndex; }

    // VrmlNode interface
  public:
    virtual bool parse(SdScanerVrml *scaner, const QString &fieldType) override;
  };

#endif // VRMLNODE1INDEXEDFACESET_H
