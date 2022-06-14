#ifndef VRMLNODE1COORDINATE3_H
#define VRMLNODE1COORDINATE3_H

#include "VrmlNode1.h"
#include "VrmlVector.h"

class VrmlNode1Coordinate3 : public VrmlNode1
  {
    VrmlVectorList mPointTable;
  public:
    VrmlNode1Coordinate3();

    bool       isPointValid( int index ) const { return index >= 0 && index < mPointTable.count(); }

    VrmlVector point( int index ) const { return mPointTable.at(index); }

    int        pointCount() const { return mPointTable.count(); }

    // VrmlNode interface
  public:
    virtual bool parse(SdScanerVrml *scaner, const QString &fieldType) override;
  };

#endif // VRMLNODE1COORDINATE3_H
