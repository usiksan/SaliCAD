#ifndef VRMLNODE1NORMAL_H
#define VRMLNODE1NORMAL_H

#include "VrmlNode1.h"
#include "VrmlVector.h"

class VrmlNode1Normal : public VrmlNode1
  {
    VrmlVectorList mVector;
  public:
    VrmlNode1Normal();

    bool       isVectorValid( int index ) const { return index >= 0 && index < mVector.count(); }

    VrmlVector vector( int index ) const { return mVector.at(index); }

    // VrmlNode interface
  public:
    virtual bool parse(SdScanerVrml *scaner, const QString &fieldType) override;
  };

#endif // VRMLNODE1NORMAL_H
